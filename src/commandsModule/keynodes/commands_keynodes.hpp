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
};
}  // namespace commandsModule
