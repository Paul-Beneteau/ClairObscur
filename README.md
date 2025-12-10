# Clair Obscur Fan Project | Combat Mechanics - Unreal Engine, C++

## Overview

Recreation of Clair Obscur combat mechanics, with one playable character that uses some of Maëlle's abilities. The focus was on implementing the core systems (like the turn logic, Input/UI context) and exploring Unreal gameplay ability system (GAS) rather than creating a lot of content/abilities.

**Combat Mechanics**
- Turn order calculated from character speed
- Dodge mechanic during enemy attacks
- Combat stances: Defensive (50% damage reduction), Offensive (1.5x damage), Virtuose (3x damage)
- Status effects: Burn (stacking DoT), Defenseless (25% increased damage taken)
- Consumable items giving Action points and Health

**Player Systems**
- Context-based input management (action selection → target selection → ability activation)
- Dynamic camera positioning depending on the context
- Target selection for abilities
  
## Architecture

- `TurnManagerSubsystem` - Game instance subsystem managing turn queue and round progression
- `ClairCharacter` - Base character class implementing GAS and turn interface
- `ClairPlayerCharacter` - Handles input contexts, camera, and target selection

## Links

- [Demo Video](https://www.youtube.com/watch?v=B-IY7ACAFIo)
- [Playable Build on Itch.io](https://paulbeneteau.itch.io/clair-obscur-combat)
- [Portfolio Website](https://paulbeneteau.com)
