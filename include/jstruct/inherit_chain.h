#pragma once

namespace jstruct {

template < typename Base, template < typename T > typename ... Fs >
class InheritChain;

template < typename Base, template < typename T > typename F,  template < typename T > typename ... Fs >
class InheritChain < Base, F, Fs ... > : public F < InheritChain < Base, Fs ... > > {};

template < typename Base, template < typename T > typename F >
class InheritChain < Base, F > : public F < Base > {};

template < typename Base >
class InheritChain < Base > : public Base {};

};
