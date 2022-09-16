#include <memory>

#include "sc-memory/sc_memory.hpp"


#include "sc-memory/sc_event.hpp"




#define ScComponentManagerKeynodes_hpp_19_init  bool _InitInternal() override \
{ \
    ScMemoryContext ctx(sc_access_lvl_make_min, "ScComponentManagerKeynodes::_InitInternal"); \
    bool result = true; \
    return result; \
}


#define ScComponentManagerKeynodes_hpp_19_initStatic static bool _InitStaticInternal()  \
{ \
    ScMemoryContext ctx(sc_access_lvl_make_min, "ScComponentManagerKeynodes::_InitStaticInternal"); \
    bool result = true; \
	concept_reusable_component = ctx.HelperResolveSystemIdtf("concept_reusable_component", ScType::NodeConstClass); result = result && concept_reusable_component.IsValid(); \
	concept_repository = ctx.HelperResolveSystemIdtf("concept_repository", ScType::NodeConstClass); result = result && concept_repository.IsValid(); \
	rrel_repositories = ctx.HelperResolveSystemIdtf("rrel_repositories", ScType::NodeConstRole); result = result && rrel_repositories.IsValid(); \
	rrel_components = ctx.HelperResolveSystemIdtf("rrel_components", ScType::NodeConstRole); result = result && rrel_components.IsValid(); \
	nrel_author = ctx.HelperResolveSystemIdtf("nrel_author", ScType::NodeConstRole); result = result && nrel_author.IsValid(); \
	nrel_explanation = ctx.HelperResolveSystemIdtf("nrel_explanation", ScType::NodeConstRole); result = result && nrel_explanation.IsValid(); \
    return result; \
}


#define ScComponentManagerKeynodes_hpp_19_decl 

#define ScComponentManagerKeynodes_hpp_ScComponentManagerKeynodes_impl 

#undef ScFileID
#define ScFileID ScComponentManagerKeynodes_hpp

