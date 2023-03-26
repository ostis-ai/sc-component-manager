/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "generated/ScComponentManagerKeynodes.generated.hpp"

namespace keynodes
{
class ScComponentManagerKeynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("concept_reusable_component"), ForceCreate(ScType::NodeConstClass))
  static ScAddr concept_reusable_component;

  SC_PROPERTY(Keynode("concept_repository"), ForceCreate(ScType::NodeConstClass))
  static ScAddr concept_repository;

  SC_PROPERTY(Keynode("concept_reusable_component_specification"), ForceCreate(ScType::NodeConstClass))
  static ScAddr concept_reusable_component_specification;

  SC_PROPERTY(Keynode("concept_single_address"), ForceCreate(ScType::NodeConstClass))
  static ScAddr concept_single_address;

  SC_PROPERTY(Keynode("concept_complex_address"), ForceCreate(ScType::NodeConstClass))
  static ScAddr concept_complex_address;

  SC_PROPERTY(Keynode("concept_github_url"), ForceCreate(ScType::NodeConstClass))
  static ScAddr concept_github_url;

  SC_PROPERTY(Keynode("concept_google_drive_url"), ForceCreate(ScType::NodeConstClass))
  static ScAddr concept_google_drive_url;

  SC_PROPERTY(Keynode("rrel_repositories_specifications"), ForceCreate(ScType::NodeConstRole))
  static ScAddr rrel_repositories_specifications;

  SC_PROPERTY(Keynode("rrel_components_specifications"), ForceCreate(ScType::NodeConstRole))
  static ScAddr rrel_components_specifications;

  // SC_PROPERTY(Keynode("rrel_address"), ForceCreate(ScType::NodeConstRole))
  // static ScAddr rrel_address;

  SC_PROPERTY(Keynode("nrel_authors"), ForceCreate(ScType::NodeConstNoRole))
  static ScAddr nrel_authors;

  SC_PROPERTY(Keynode("nrel_explanation"), ForceCreate(ScType::NodeConstNoRole))
  static ScAddr nrel_explanation;

  SC_PROPERTY(Keynode("nrel_component_address"), ForceCreate(ScType::NodeConstNoRole))
  static ScAddr nrel_component_address;

  SC_PROPERTY(Keynode("nrel_component_dependencies"), ForceCreate(ScType::NodeConstNoRole))
  static ScAddr nrel_component_dependencies;

  SC_PROPERTY(Keynode("nrel_installation_method"), ForceCreate(ScType::NodeConstNoRole))
  static ScAddr nrel_installation_method;

  SC_PROPERTY(Keynode("nrel_alternative_addresses"), ForceCreate(ScType::NodeConstNoRole))
  static ScAddr nrel_alternative_addresses;

  SC_PROPERTY(Keynode("nrel_repository_address"), ForceCreate(ScType::NodeConstNoRole))
  static ScAddr nrel_repository_address;

  SC_PROPERTY(Keynode("nrel_install_scripts"), ForceCreate(ScType::NodeConstNoRole))
  static ScAddr nrel_install_scripts;
};

}  // namespace keynodes
