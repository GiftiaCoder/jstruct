#pragma once

#include "inherit_chain.h"

// allocer
#include "allocer/allocer_base.h"
#include "allocer/align_allocer.h"

// plugin
#include "allocer/str_dedup_plugin.h"

namespace jstruct {

template < typename Base, template < typename B > typename ... FuncPlugin >
class Allocer : public InheritChain < Base, FuncPlugin ... > {};

// A8 : all data of align 8
// is block_size better to be 4K(page_size) or 2M(huge_page_size) ?
template < template < typename B > typename ... FuncPlugin >
class A8Allocer : public Allocer < AlignAllocer, FuncPlugin ... > {};

} // ns jstruct
