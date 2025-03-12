//
// Copyright (c) 2024.
// Author: Joran Vandenbroucke.
//

#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <utility>

#include "aabb.hpp"
#include "ray.hpp"

import FawnAlgebra;

namespace FawnAlgebra
{
inline constexpr auto BINS{8};

template <typename Type, std::uint8_t Dimension>
struct Node
{
    aabb<float, Dimension> boundingBox{};
    std::uint32_t          leftFirst{};
    std::uint32_t          objCount{};
};
using Node2D = Node<float, 2>;
using Node3D = Node<float, 3>;

template <typename Type, std::uint8_t Dimension>
[[nodiscard]] constexpr bool IsLeaf(const Node<Type, Dimension>& node) noexcept
{
    // empty BVH leaves do not exist
    return node.objCount > 0;
}

template <typename NodeN, std::uint8_t Dimension>
[[nodiscard]] constexpr Type CalculateNodeCost(const NodeN& node) noexcept
{
    const auto delta{node.boundingBox.bMax - node.boundingBox.bMin};

    float surfaceAreaSum{};
    for (std::uint8_t i {}; i < Dimension; ++i)
    {
        float faceArea{1.0f};
        for (std::uint8_t j = 0; j < Dimension; ++j)
        {
            if (i != j)
            {
                faceArea *= delta.coord[j];
            }
        }
        surfaceAreaSum += faceArea;
    }

    return surfaceAreaSum * static_cast<float>(node.objCount);
}

[[nodiscard]] constexpr Type CalculateNodeCost(const Node2D& node) noexcept
{
    const float2 delta{node.boundingBox.bMax - node.boundingBox.bMin};
    return delta.coord.x * delta.coord.y * static_cast<float>(node.objCount);
}

[[nodiscard]] constexpr Type CalculateNodeCost(const Node3D& node) noexcept
{
    const Vec<Type, Dimension> delta{node.boundingBox.bMax - node.boundingBox.bMin};
    return (delta.coord.x * delta.coord.y + delta.coord.y * delta.coord.z + delta.coord.z * delta.coord.x)
           * static_cast<float>(node.objCount);
}

// the "ULTIMATE" goal is to have an enum called tree_type or space_tree_type
// this is followed with create structs something like faSpaceTreeCreateInfo or faBVHCreateInfo, and faBSPCreateInfo
// these structs contain the correct data to initialize the tree
//
// We assume that we get a list of centroids
// If needed, we can explicitly overload this function for when we don't do this, e.g. triangles.
// I guess we can go all the way to the bottom and write
// class BVH<Type, Dimension, std::vector<triangles>>() or class BVH<Type, Dimension, Mesh>()
template <typename Type, std::uint8_t Dimension, HasSizeAndDataOrIsArray Container>
class BoundingVolumeHierarchy
{
  public:
    BoundingVolumeHierarchy(const Container* pContainer) :
        m_pContainer{pContainer},
        m_bvhNode{new(std::align_val_t(64)) Node<Type, Dimension>[std::size(*pContainer) * 2 + 64]},
        m_objIndex{new uint32_t[std::size(*pContainer)]} m_nodesUsed{2},
        m_getSize{getSize}
    {
        Build();
    }
    ~BoundingVolumeHierarchy()                                         = default;
    BoundingVolumeHierarchy(const BoundingVolumeHierarchy&)            = default;
    BoundingVolumeHierarchy(BoundingVolumeHierarchy&&)                 = default;
    BoundingVolumeHierarchy& operator=(const BoundingVolumeHierarchy&) = default;
    BoundingVolumeHierarchy& operator=(BoundingVolumeHierarchy&&)      = default;

    void Refit() noexcept
    {
        for (uint32_t i{m_nodesUsed - 1}; i >= 0U; --i)
            if (i != 1)
            {
                BVHNode& node{m_bvhNode[i]};
                if (node.IsLeaf())
                {
                    // leaf node: adjust bounds to contained triangles
                    Vec<Type, Dimension> dummy1{}; // we don't need centroid bounds here
                    Vec<Type, Dimension> dummy2{}; // we don't need centroid bounds here
                    UpdateNodeBounds(i, dummy1, dummy2);
                    continue;
                }
                // interior node: adjust bounds to child node bounds
                BVHNode& leftChild{bvhNode[node.leftFirst]};
                BVHNode& rightChild{bvhNode[node.leftFirst + 1]};
                node.aabbMin = min(leftChild.aabbMin, rightChild.aabbMin);
                node.aabbMax = max(leftChild.aabbMax, rightChild.aabbMax);
            }
    }
    bool Intersect(const aabb<Type, Dimension>& boundingBox, uint32_t instanceIdx) noexcept
    {
        Node<Type, Dimension>* node{&m_bvhNode[0]}; // Start at root node
        Node<Type, Dimension>* stack[64];
        uint32_t               stackPtr{};

        while (true)
        {
            if (node->IsLeaf())
            {
                for (uint32_t i{}; i < node->objCount; ++i)
                {
                    const uint32_t instPrim{(instanceIdx << 20) + m_objIndex[node->leftFirst + i]};

                    // NOTE : we might be happy with a leaf node intersecting, the we should use `boundingBox.Intersect(node->boundingBox)`
                    // using Intersect might also be more wanted for a `GetAllWithin(const aabb&, uint32_t)` function ¯\_(ツ)_/¯

                    // Check AABB intersection with stored object
                    if (boundingBox.Contains(m_pContainer[instPrim & 0xffff].boundingBox))
                    {
                        return true; // Early exit if intersection is found
                    }
                }

                if (stackPtr == 0)
                {
                    break;
                }
                else
                {
                    node = stack[--stackPtr];
                }
            }
            else
            {
                const Node<Type, Dimension>* child1{&m_bvhNode[node->leftFirst]};
                const Node<Type, Dimension>* child2{&m_bvhNode[node->leftFirst + 1]};

                const bool overlap1{boundingBox.Intersect(child1->boundingBox))};
                const bool overlap2{boundingBox.Intersect(child2->boundingBox))};

                if (overlap1 && overlap2)
                {
                    // Traverse both children, push farthest first
                    stack[stackPtr++] = child2;
                    node              = child1;
                }
                else if (overlap1)
                {
                    node = child1;
                }
                else if (overlap2)
                {
                    node = child2;
                }
                else
                {
                    if (stackPtr == 0)
                    {
                        break;
                    }
                    else
                    {
                        node = stack[--stackPtr];
                    }
                }
            }
        }
        return false; // No intersection found
    }
    constexpr auto Intersect(const Ray<Type, Dimension>& ray, uint32_t instanceIdx) noexcept -> bool
    {
        Node<Type, Dimension>* node{&m_bvhNode[0]};
        Node<Type, Dimension>* stack[64]{};

        uint32_t stackPtr{};
        while (true)
        {
            if (node->IsLeaf())
            {
                for (i{}; i < node->objCount; ++i)
                {
                    const uint32_t instPrim{(instanceIdx << 20) + m_objIndex[node->leftFirst + i]};
                    IntersectObject(boundingBox, m_pContainer[instPrim & 0xffff], instPrim);
                }
                if (stackPtr == 0)
                {
                    break;
                }
                else
                {
                    node = stack[--stackPtr];
                }
            }

            const Node<Type, Dimension>* child1{m_bvhNode[node->leftFirst]};
            const Node<Type, Dimension>* child2{m_bvhNode[node->leftFirst + 1]};

            const float dist1{IntersectAABB(ray, child1->boundingBox)};
            const float dist2{IntersectAABB(ray, child2->boundingBox)};

            if (dist2 < dist1)
            {
                using std::swap;
                swap(dist1, dist2);
                swap(child1, child2);
            }
            if (dst1 == std::numeric_limits<Type>::max())
            {
                if (stackPtr == 0)
                {
                    break;
                }
                else
                {
                    node = stack[--stackPtr];
                }
            }
            else
            {
                node = child1;
                if (dist2 != std::numeric_limits<Type>::max())
                {
                    stack[stackPtr++] = child2;
                }
            }
        }
    }

  private:
    struct BuildJob
    {
        uint32_t             nodeIndex{};
        Vec<Type, Dimension> centroidMin{};
        Vec<Type, Dimension> centroidMax{};
    };

    Node<Type, Dimension>* m_bvhNode{nullptr};
    Container              m_pContainer{nullptr};
    uint32_t*              m_objIndex{nullptr};
    uint32_t               m_nodesUsed{};
    bool                   m_subdivToOnePrim{false};

    constexpr auto IntersectAABB(const Ray<Type, Dimension>& ray, const Vec<Type, Dimension>& bmin, const Vec<Type, Dimension>& bmax)
        -> Type
    {
        // "slab test" ray/AABB intersection
        Vec<Type, Dimension> t1{bmin - ray.origine) * ray.reciprocalDirection};
        Vec<Type, Dimension> t2{bmax - ray.origine) * ray.reciprocalDirection};

        Type tmin{min(t1[0], t2[0])};
        Type tmax{max(t1[0], t2[0])};
        for (std::uint8_t i{1}; i < Dimension; ++i)
        {
            tmin = max(tmin, min(t1[i], t2[i]));
            tmax = min(tmin, max(t1[i], t2[i]));
        }

        if (tmax >= tmin && tmin < ray.hit.t && tmax > 0)
        {
            return tmin;
        }
        return std::numeric_limits<Type>::max();
    }
    constexpr void UpdateNodeBounds(uint32_t nodeIdx, const Vec<Type, Dimension>& centroidMin, const Vec<Type, Dimension>& centroidMax)
    {
        Node<Type, Dimension>& node{m_bvhNode[nodeIdx]};
        node.boundingBox.bMin = std::numeric_limits<Type>::max();
        node.boundingBox.bMax = std::numeric_limits<Type>::min();
        centroidMin           = std::numeric_limits<Type>::max();
        centroidMax           = std::numeric_limits<Type>::min();
        for (uint32_t first{node.leftFirst}, i{}; i < node.objCount; ++i)
        {
            uint32_t leafObjIndex{m_objIndex[first + i]};

            const Vec<Type, Dimension>& current{m_pContainer[leafObjIndex]};
            node.boundingBox.Grow(current);
            centroidMin = min(centroidMin, current);
            centroidMax = max(centroidMax, current);
        }
    }
    constexpr auto FindBestSplitPlane(const Node<Type, Dimension>& node,
                                      const uint32_t&              axis,
                                      const uint32_t&              splitPos,
                                      const Vec<Type, Dimension>&  centroidMin,
                                      const Vec<Type, Dimension>&  centroidMax) -> Type
    {
        const Type bestCost{std::numeric_limits<Type>::max()};
        for (int a{}; a < Dimension; a++)
        {
            const Type boundsMin{centroidMin[a]};
            const Type boundsMax{centroidMax[a]};
            if (boundsMin == boundsMax)
            {
                continue;
            }
            // populate the bins
            float scale{BINS / (boundsMax - boundsMin)};

            std::array<Type, BINS - 1U> leftCountArea;
            std::array<Type, BINS - 1U> rightCountArea;

            int leftSum{};
            int rightSum{};

            struct Bin
            {
                aabb<Typem, Dimension> bounds{};
                int                    objCount{};
            } bin[BINS];

            for (uint i{}; i < node.objCount; i++)
            {
                const Vec<Type, Dimension>& obj{m_pContainer[m_objIndex[node.leftFirst + i]]};

                const int binIdx{min(BINS - 1, (int) ((obj.centroid[a] - boundsMin) * scale))};
                bin[binIdx].objCount++;
                bin[binIdx].bounds.Grow(obj);
                // NOTE : when we ACTUALLY ant triangles, we will have to replace this part with the below three lines,
                // the above thingy is just the "centroid" or the "pivot point"
                // bin[binIdx].bounds.grow(obj.vertex0);
                // bin[binIdx].bounds.grow(obj.vertex1);
                // bin[binIdx].bounds.grow(obj.vertex2);
            }

            // gather data for the 7 planes between the 8 bins
            aabb<Typem, Dimension> leftBox, rightBox;
            for (int i{}; i < BINS - 1; i++)
            {
                leftSum += bin[i].objCount;
                leftBox.Grow(bin[i].bounds);
                leftCountArea[i] = leftSum * leftBox.Area();
                rightSum += bin[BINS - 1 - i].objCount;
                rightBox.Grow(bin[BINS - 1 - i].bounds);
                rightCountArea[BINS - 2 - i] = rightSum * rightBox.Area();
            }
            // calculate SAH cost for the 7 planes
            scale = (boundsMax - boundsMin) / BINS;
            for (int i = 0; i < BINS - 1; i++)
            {
                const Type planeCost{leftCountArea[i] + rightCountArea[i]};
                if (planeCost < bestCost)
                    axis = a, splitPos = i + 1, bestCost = planeCost;
            }
        }
        return bestCost;
    }
    constexpr void Subdevide(uint32_t                    nodeIndex,
                             uint32_t                    depth,
                             uint32_t&                   nodePtr,
                             const Vec<Type, Dimension>& centroidMin,
                             const Vec<Type, Dimension>& centroidMax)
    {
        Node<Type, Dimension>& node{m_bvhNode[nodeIndex]};

        uint32_t axis{};
        uint32_t splitPosition{};
        Type     splitCost{FindBestSplitPlane(node, axis, splitPos, centroidMin, centroidMax)};
        if (m_subdivToOnePrim)
        {
            if (node.objCount == 1)
            {
                return;
            }
        }
        else
        {
            const Type nosplitCost{node.CalculateNodeCost()};
            if (splitCost >= nosplitCost)
            {
                return;
            }
        }
        uint32_t    i{node.leftFirst};
        uint32_t    j{node.leftFirst + node.objCount - 1};
        const float scale{static_cast<Type>(BINS) / (centroidMax[axis] - centroidMin[axis])};
        while (i <= j)
        {
            // this method for binning is done to prevent  rare inaccuracies
            const uint32_t binIdx{min(BINS - 1, static_cast<uint32_t>(m_pContainer[i][axis] - centroidMin[axis] * scale))};
            if (binIdx < splitPosition)
            {
                ++i;
            }
            else
            {
                using std::swap;
                swap(m_objIndex[i], m_objIndex[j--]);
            }
        }
        const uint32_t leftCount{i - node.leftFirst};
        if (leftCount == 0 || leftCount == node.objCount)
        {
            return; // never happens for dragon mesh, nice
        }
        // create child nodes
        uint32_t leftChildIdx{nodePtr++};
        uint32_t rightChildIdx{nodePtr++};
        m_bvhNode[leftChildIdx].leftFirst  = node.leftFirst;
        m_bvhNode[leftChildIdx].objCount   = leftCount;
        m_bvhNode[rightChildIdx].leftFirst = i;
        m_bvhNode[rightChildIdx].objCount  = node.objCount - leftCount;

        node.leftFirst = leftChildIdx;
        node.objCount  = 0;

        // recursive
        UpdateNodeBounds(leftChildIdx, centroidMin, centroidMax);
        Subdivide(leftChildIdx, depth + 1U, nodePtr, centroidMin, centroidMax);
        UpdateNodeBounds(rightChildIdx, centroidMin, centroidMax);
        Subdivide(rightChildIdx, depth + 1U, nodePtr, centroidMin, centroidMax);
    }
    constexpr void Build() noexcept
    {
        const uint32_t count{std::size(*m_pContainer)};
        m_nodesUsed = 2U;
        std::memset(m_bvhNode, 0, count * 2U * sizeof(Vec<Type, Dimension>));
        std::memset(m_objIndex, 0, count * sizeof(uint32_t));

        Node<Type, Dimension>& rootNode{m_bvhNode[]};
        Vec<Type, Dimension>   centroidMin{};
        Vec<Type, Dimension>   centroidMax{};

        UpdateNodeBounds(0, centroidMin, centroidMax);
        Subdevide(0, 0, m_nodesUsed, centroidMin, centroidMax);
    }
} template <HasSizeAndDataOrIsArray Container>
using bvh2d_float32 = BoundingVolumeHierarchy<float, 2, Container>;
template <HasSizeAndDataOrIsArray Container>
using bvh3d_float32 = BoundingVolumeHierarchy<float, 3, Container>;

template <HasSizeAndDataOrIsArray Container>
using bvh2d = bvh2d_float32;
template <HasSizeAndDataOrIsArray Container>
using bvh3d = bvh3d_float32;
} // namespace FawnAlgebra
