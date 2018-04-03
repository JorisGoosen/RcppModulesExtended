// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-
//
// Rcpp_init.cpp : Rcpp R/C++ interface class library -- Initialize and register
//
// Copyright (C) 2010 - 2017  John Chambers, Dirk Eddelbuettel and Romain Francois
//
// This file is part of Rcpp.
//
// Rcpp is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Rcpp is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Rcpp.  If not, see <http://www.gnu.org/licenses/>.

#define COMPILING_RCPP

#include <Rcpp.h>
#include "internal.h"
#include "RcppModulesExtended/Module.h"

// borrowed from Matrix
#define CALLDEF(name, n)  {#name, (DL_FUNC) &name, n}
#define EXTDEF(name)  {#name, (DL_FUNC) &name, -1}

static R_CallMethodDef callEntries[]  = {
    CALLDEF(Class__name,1),
    CALLDEF(Class__has_default_constructor,1),

    CALLDEF(CppClass__complete,1),
    CALLDEF(CppClass__methods,1),

    CALLDEF(CppObject__finalize,2),

    CALLDEF(Module__classes_info,1),
    CALLDEF(Module__complete,1),
    CALLDEF(Module__get_class,2),
    CALLDEF(Module__has_class,2),
    CALLDEF(Module__has_function,2),
    CALLDEF(Module__functions_arity,1),
    CALLDEF(Module__functions_names,1),
    CALLDEF(Module__name,1),
    CALLDEF(Module__get_function, 2),

    CALLDEF(CppField__get,3),
    CALLDEF(CppField__set,4),

    {NULL, NULL, 0}
};

static R_ExternalMethodDef extEntries[]  = {
    EXTDEF(CppMethod__invoke),
    EXTDEF(CppMethod__invoke_void),
    EXTDEF(CppMethod__invoke_notvoid),
    EXTDEF(CppMethod__invoke_pairlist),
    EXTDEF(CppMethod__invoke_void_pairlist),
    EXTDEF(CppMethod__invoke_notvoid_pairlist),
    EXTDEF(InternalFunction_invoke),
    EXTDEF(Module__invoke),
    EXTDEF(class__newInstance),
    EXTDEF(class__dummyInstance),

    {NULL, NULL, 0}
};

// this is called by R_init_Rcpp that is in Module.cpp
void init_Rcpp_routines(DllInfo *info){
    // Register routines, allocate resources.
    R_registerRoutines(info,
                       NULL,            // .C
                       callEntries,     // .Call
                       NULL,            // .Fortran
                       extEntries       // .External
                       );
}

void registerExtendedFunctions(){
    using namespace Rcpp;
    using namespace Rcpp::internal;
    using namespace RcppModulesExtended;

    #define RCPP_REGISTER(__FUN__) R_RegisterCCallable( "Rcpp", #__FUN__ , (DL_FUNC)__FUN__ );
    RCPP_REGISTER(getCurrentScopeExtended)
    RCPP_REGISTER(setCurrentScopeExtended)
    R_RegisterCCallable( "Rcpp", "getCurrentScope" , (DL_FUNC)getCurrentScopeExtended );
    R_RegisterCCallable( "Rcpp", "setCurrentScope" , (DL_FUNC)setCurrentScopeExtended );
    // RCPP_REGISTER(print)
    #undef RCPP_REGISTER
}


extern "C" void R_unload_Rcpp(DllInfo *) {  // #nocov start
    // Release resources
} 						// #nocov end

extern "C" void R_init_Rcpp(DllInfo* dllinfo) {
    setCurrentScope(0);

    registerExtendedFunctions();       // call wrapper to register export symbols 

    R_useDynamicSymbols(dllinfo, FALSE);	// set up symbol symbol lookup (cf R 3.4.0)

    //init_Rcpp_cache();          		// init the cache

    init_Rcpp_routines(dllinfo);		// init routines
}
