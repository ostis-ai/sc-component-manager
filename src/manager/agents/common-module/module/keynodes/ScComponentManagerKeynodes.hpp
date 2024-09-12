/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_keynodes.hpp>

namespace keynodes
{
class ScComponentManagerKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const myself{"myself", ScType::NodeConst};

  static inline ScKeynode const concept_reusable_component{"concept_reusable_component", ScType::NodeConstClass};

  static inline ScKeynode const current_components_to_install{
      "concept_current_components_to_install",
      ScType::NodeConst};

  static inline ScKeynode const concept_reusable_kb_component{"concept_reusable_kb_component", ScType::NodeConstClass};

  static inline ScKeynode const concept_reusable_ps_component{"concept_reusable_ps_component", ScType::NodeConstClass};

  static inline ScKeynode const concept_reusable_ui_component{"concept_reusable_ui_component", ScType::NodeConstClass};

  static inline ScKeynode const concept_reusable_embedded_ostis_system{
      "concept_reusable_embedded_ostis_system",
      ScType::NodeConstClass};

  static inline ScKeynode const concept_repository{"concept_repository", ScType::NodeConstClass};

  static inline ScKeynode const concept_reusable_component_specification{
      "concept_reusable_component_specification",
      ScType::NodeConstClass};

  static inline ScKeynode const concept_single_address{"concept_single_address", ScType::NodeConstClass};

  static inline ScKeynode const concept_complex_address{"concept_complex_address", ScType::NodeConstClass};

  static inline ScKeynode const concept_github_url{"concept_github_url", ScType::NodeConstClass};

  static inline ScKeynode const concept_need_to_install_components{
      "concept_need_to_install_components",
      ScType::NodeConstClass};

  static inline ScKeynode const concept_google_drive_url{"concept_google_drive_url", ScType::NodeConstClass};

  static inline ScKeynode const sc_model_of_knowledge_base{"sc_model_of_knowledge_base", ScType::NodeConstClass};

  static inline ScKeynode const sc_model_of_interface{"sc_model_of_interface", ScType::NodeConstClass};

  static inline ScKeynode const sc_model_of_problem_solver{"sc_model_of_problem_solver", ScType::NodeConstClass};

  static inline ScKeynode const concept_subsystems_set{"concept_subsystems_set", ScType::NodeConstClass};

  static inline ScKeynode const rrel_repositories_specifications{
      "rrel_repositories_specifications",
      ScType::NodeConstRole};

  static inline ScKeynode const rrel_components_specifications{"rrel_components_specifications", ScType::NodeConstRole};

  static inline ScKeynode const nrel_authors{"nrel_authors", ScType::NodeConstNoRole};

  static inline ScKeynode const nrel_explanation{"nrel_explanation", ScType::NodeConstNoRole};

  static inline ScKeynode const nrel_note{"nrel_note", ScType::NodeConstNoRole};

  static inline ScKeynode const nrel_purpose{"nrel_purpose", ScType::NodeConstNoRole};

  static inline ScKeynode const nrel_key_sc_element{"nrel_key_sc_element", ScType::NodeConstNoRole};

  static inline ScKeynode const nrel_component_address{"nrel_component_address", ScType::NodeConstNoRole};

  static inline ScKeynode const nrel_component_dependencies{"nrel_component_dependencies", ScType::NodeConstNoRole};

  static inline ScKeynode const nrel_installation_method{"nrel_installation_method", ScType::NodeConstNoRole};

  static inline ScKeynode const nrel_alternative_addresses{"nrel_alternative_addresses", ScType::NodeConstNoRole};

  static inline ScKeynode const nrel_repository_address{"nrel_repository_address", ScType::NodeConstNoRole};

  static inline ScKeynode const nrel_installation_script{"nrel_installation_script", ScType::NodeConstNoRole};

  static inline ScKeynode const nrel_decomposition{"nrel_decomposition", ScType::NodeConstNoRole};

  static inline ScKeynode const nrel_ostis_system_decomposition{
      "nrel_ostis_system_decomposition",
      ScType::NodeConstNoRole};

  static inline ScKeynode const action_components_init{"action_components_init", ScType::NodeConstClass};

  static inline ScKeynode const action_components_search{"action_components_search", ScType::NodeConstClass};

  static inline ScKeynode const action_components_install{"action_components_install", ScType::NodeConstClass};

  static inline ScKeynode const rrel_author{"rrel_author", ScType::NodeConstRole};

  static inline ScKeynode const rrel_class{"rrel_class", ScType::NodeConstRole};

  static inline ScKeynode const rrel_sets{"rrel_sets", ScType::NodeConstRole};

  static inline ScKeynode const rrel_components{"rrel_components", ScType::NodeConstRole};

  static inline ScKeynode const rrel_explanation{"rrel_explanation", ScType::NodeConstRole};

  static inline ScKeynode const rrel_identifier{"rrel_identifier", ScType::NodeConstRole};

  static inline ScKeynode const rrel_note{"rrel_note", ScType::NodeConstNoRole};

  static inline ScKeynode const rrel_purpose{"rrel_purpose", ScType::NodeConstNoRole};

  static inline ScKeynode const rrel_main_idtf{"rrel_main_idtf", ScType::NodeConstNoRole};
};

}  // namespace keynodes
