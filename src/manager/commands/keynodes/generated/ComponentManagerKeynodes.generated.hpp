#include <memory>

#include "sc-memory/sc_memory.hpp"


#include "sc-memory/sc_event.hpp"




#define ComponentManagerKeynodes_hpp_20_init  bool _InitInternal() override \
{ \
    ScMemoryContext ctx(sc_access_lvl_make_min, "ComponentManagerKeynodes::_InitInternal"); \
    bool result = true; \
    return result; \
}


#define ComponentManagerKeynodes_hpp_20_initStatic static bool _InitStaticInternal()  \
{ \
    ScMemoryContext ctx(sc_access_lvl_make_min, "ComponentManagerKeynodes::_InitStaticInternal"); \
    bool result = true; \
	concept_reusable_component = ctx.HelperResolveSystemIdtf("concept_reusable_component", ScType::NodeConstClass); result = result && concept_reusable_component.IsValid(); \
	concept_repository = ctx.HelperResolveSystemIdtf("concept_repository", ScType::NodeConstClass); result = result && concept_repository.IsValid(); \
	rrel_repositories = ctx.HelperResolveSystemIdtf("rrel_repositories", ScType::NodeConstRole); result = result && rrel_repositories.IsValid(); \
	rrel_components = ctx.HelperResolveSystemIdtf("rrel_components", ScType::NodeConstRole); result = result && rrel_components.IsValid(); \
    return result; \
}


#define ComponentManagerKeynodes_hpp_20_decl 

#define ComponentManagerKeynodes_hpp_ComponentManagerKeynodes_impl 

#undef ScFileID
#define ScFileID ComponentManagerKeynodes_hpp

