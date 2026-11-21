#pragma once
#include "berialdraw.hpp"

#ifdef _
#error "language_gettext.hpp cannot be included together with another definition of _"
#endif

#define _(id) ::berialdraw::UIManager::language()->translate(id)
