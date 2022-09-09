/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "generated/ComponentManagerKeynodes.generated.hpp"

namespace keynodes
{

class ComponentManagerKeynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("concept_reusable_component"), ForceCreate(ScType::NodeConstClass))
  static ScAddr concept_reusable_component;

  SC_PROPERTY(Keynode("concept_repository"), ForceCreate(ScType::NodeConstClass))
  static ScAddr concept_repository;

  SC_PROPERTY(Keynode("rrel_repositories"), ForceCreate(ScType::NodeConstRole))
  static ScAddr rrel_repositories;

  SC_PROPERTY(Keynode("rrel_components"), ForceCreate(ScType::NodeConstRole))
  static ScAddr rrel_components;
};

} // namespace commands
