//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once

#define QUOTE_(x) #x
#define QUOTE(x) QUOTE_(x)
#define FILE_LINE_ __FILE__ "(" QUOTE(__LINE__) ") : "

#define NOTE(x) message(x)
#define FILE_LINE message(FILE_LINE_)

#define TODO(x)                                                                                                                                                                                                                                \
    message(FILE_LINE_ "\n"                                                                                                                                                                                                                    \
                       " ------------------------------------------------\n"                                                                                                                                                                   \
                       "|  TODO :   " #x "\n"                                                                                                                                                                                                  \
                       " -------------------------------------------------\n")
#define FIXME(x)                                                                                                                                                                                                                               \
    message(FILE_LINE_ "\n"                                                                                                                                                                                                                    \
                       " ------------------------------------------------\n"                                                                                                                                                                   \
                       "|  FIXME :  " #x "\n"                                                                                                                                                                                                  \
                       " -------------------------------------------------\n")
#define todo(x) message(FILE_LINE_ " TODO :   " #x "\n")
#define fixme(x) message(FILE_LINE_ " FIXME:   " #x "\n")
