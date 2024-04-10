/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "ScComponentManagerKeynodes.generated.hpp"

namespace keynodes
{
class ScComponentManagerKeynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("myself"), ForceCreate(ScType::NodeConstClass))
  static ScAddr myself;

  SC_PROPERTY(Keynode("concept_reusable_component"), ForceCreate(ScType::NodeConstClass))
  static ScAddr concept_reusable_component;

  SC_PROPERTY(Keynode("concept_reusable_kb_component"), ForceCreate(ScType::NodeConstClass))
  static ScAddr concept_reusable_kb_component;

  SC_PROPERTY(Keynode("concept_reusable_ps_component"), ForceCreate(ScType::NodeConstClass))
  static ScAddr concept_reusable_ps_component;

  SC_PROPERTY(Keynode("concept_reusable_ui_component"), ForceCreate(ScType::NodeConstClass))
  static ScAddr concept_reusable_ui_component;

  SC_PROPERTY(Keynode("concept_reusable_embedded_ostis_system"), ForceCreate(ScType::NodeConstClass))
  static ScAddr concept_reusable_embedded_ostis_system;

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

  SC_PROPERTY(Keynode("concept_need_to_install_components"), ForceCreate(ScType::NodeConstClass))
  static ScAddr concept_need_to_install_components;

  SC_PROPERTY(Keynode("concept_google_drive_url"), ForceCreate(ScType::NodeConstClass))
  static ScAddr concept_google_drive_url;

  SC_PROPERTY(Keynode("sc_model_of_knowledge_base"), ForceCreate(ScType::NodeConstClass))
  static ScAddr sc_model_of_knowledge_base;

  SC_PROPERTY(Keynode("sc_model_of_interface"), ForceCreate(ScType::NodeConstClass))
  static ScAddr sc_model_of_interface;

  SC_PROPERTY(Keynode("sc_model_of_problem_solver"), ForceCreate(ScType::NodeConstClass))
  static ScAddr sc_model_of_problem_solver;

  SC_PROPERTY(Keynode("concept_subsystems_set"), ForceCreate(ScType::NodeConstClass))
  static ScAddr concept_subsystems_set;

  SC_PROPERTY(Keynode("rrel_repositories_specifications"), ForceCreate(ScType::NodeConstRole))
  static ScAddr rrel_repositories_specifications;

  SC_PROPERTY(Keynode("rrel_components_specifications"), ForceCreate(ScType::NodeConstRole))
  static ScAddr rrel_components_specifications;

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

  SC_PROPERTY(Keynode("nrel_installation_script"), ForceCreate(ScType::NodeConstNoRole))
  static ScAddr nrel_installation_script;

  SC_PROPERTY(Keynode("nrel_decomposition"), ForceCreate(ScType::NodeConstNoRole))
  static ScAddr nrel_decomposition;

  SC_PROPERTY(Keynode("nrel_ostis_system_decomposition"), ForceCreate(ScType::NodeConstNoRole))
  static ScAddr nrel_ostis_system_decomposition;

  SC_PROPERTY(Keynode("action_components_init"), ForceCreate)
  static ScAddr action_components_init;

  SC_PROPERTY(Keynode("action_components_search"), ForceCreate)
  static ScAddr action_components_search;

  SC_PROPERTY(Keynode("action_components_install"), ForceCreate)
  static ScAddr action_components_install;

  SC_PROPERTY(Keynode("rrel_author"), ForceCreate)
  static ScAddr rrel_author;

  SC_PROPERTY(Keynode("rrel_class"), ForceCreate)
  static ScAddr rrel_class;

  SC_PROPERTY(Keynode("rrel_explanation"), ForceCreate)
  static ScAddr rrel_explanation;

  SC_PROPERTY(Keynode("rrel_identifier"), ForceCreate)
  static ScAddr rrel_identifier;
};

}  // namespace keynodes
