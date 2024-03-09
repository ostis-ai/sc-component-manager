#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "commands_keynodes.generated.hpp"

namespace commandsModule
{
class CommandsKeynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
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
}  // namespace commandsModule
