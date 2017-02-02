/*
 * This file is part of the Colobot: Gold Edition source code
 * Copyright (C) 2001-2016, Daniel Roux, EPSITEC SA & TerranovaTeam
 * http://epsitec.ch; http://colobot.info; http://github.com/colobot
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://gnu.org/licenses
 */

#pragma once

#include <string>

/**
 * \file sound/sound_type.h
 * \brief Defines the SoundType enum
 */

/**
 * \public
 * \enum    SoundType
 * \brief   Enum representing sound file
**/
enum SoundType
{
  SOUND_NONE = -1,      /*!< Silence. */
  SOUND_CLICK = 0,      /*!< Keybinding change. */
  SOUND_BOUM  = 1,      /*!< Landing on ground bump. */
  SOUND_EXPLO  = 2,     /*!< Building explosion. */
  SOUND_FLYh  = 3,      /*!< Jet engine (SpaceShip) landing. */
  SOUND_FLY  = 4,       /*!< Jet engine (SpaceShip & WingedBot) flying. */
  SOUND_STEPs  = 5,     /*!< Step on grass/mud. */
  SOUND_MOTORw  = 6,    /*!< WheeledBot engine. */
  SOUND_MOTORt  = 7,    /*!< TrackedBot engine. */
  SOUND_MOTORr  = 8,    /*!< SpecialBot engine. */
  SOUND_ERROR  = 9,     /*!< DisplayError message. */
  SOUND_CONVERT  = 10,  /*!< Converter processing. */
  SOUND_ENERGY  = 11,   /*!< PowerStation recharging #1/Menu particle #1. */
  SOUND_PLOUF  = 12,    /*!< Water splash. */
  SOUND_BLUP  = 13,     /*!< Swimming #1. */
  SOUND_WARNING  = 14,  /*!< DisplayWarning message. */
  SOUND_DERRICK  = 15,  /*!< Derrick digging. */
  SOUND_LABO  = 16,     /*!< AutoLab processing sample. */
  SOUND_STATION  = 17,  /*!< PowerStation recharging #2/Menu particle #2. */
  SOUND_REPAIR  = 18,   /*!< RepairCenter/Sniffer sniffing. */
  SOUND_RESEARCH  = 19, /*!< ResearchCenter processing. */
  SOUND_INSECTs  = 20,  /*!< AlienSpider roar. */
  SOUND_BURN  = 21,     /*!< Burning. */
  SOUND_TZOING  = 22,   /*!< EPSITEC's Brand "TZOING!". */
  SOUND_GGG  = 23,      /*!< Electricity/Menu particle #3. */
  SOUND_MANIP  = 24,    /*!< Bot mechanic arm manipulating. */
  SOUND_FIRE  = 25,     /*!< ShooterBot shooting. */
  SOUND_HUMAN1  = 26,   /*!< Astronaut breathing. */
  SOUND_STEPw  = 27,    /*!< Step under the water. */
  SOUND_SWIM  = 28,     /*!< Swimming #2. */
  SOUND_RADAR  = 29,    /*!< RadarStation "DING!"/Menu particle #4. */
  SOUND_BUILD  = 30,    /*!< Building processing. */
  SOUND_ALARM  = 31,    /*!< Bot energy alarm/Menu particle #5. */
  SOUND_SLIDE  = 32,    /*!< WingedBot engine. */
  SOUND_EXPLOi  = 33,   /*!< AlienInsect exploding scream. */
  SOUND_INSECTa  = 34,  /*!< AlienAnt roar. */
  SOUND_INSECTb  = 35,  /*!< AlienWasp roar. */
  SOUND_INSECTw  = 36,  /*!< AlienWorm roar. */
  SOUND_INSECTm  = 37,  /*!< AlienQueen roar. */
  SOUND_TREMBLE  = 38,  /*!< Building ambient. */
  SOUND_PSHHH  = 39,    /*!< SpecialBot engine "PSHHH!"/Menu particle #6. */
  SOUND_NUCLEAR  = 40,  /*!< NuclearPlant processing. */
  SOUND_INFO  = 41,     /*!< DisplayInfo message/Menu particle #7. */
  SOUND_OPEN  = 42,     /*!< Converter open/close. */
  SOUND_CLOSE  = 43,    /*!< Converter slam. */
  SOUND_FACTORY  = 44,  /*!< BotFactory processing. */
  SOUND_EGG  = 45,      /*!< AlienEgg breaking eggshell. */
  SOUND_MOTORs  = 46,   /*!< Subber engine. */
  SOUND_MOTORi  = 47,   /*!< LeggedBot engine. */
  SOUND_SHIELD  = 48,   /*!< Shielder shielding. */
  SOUND_FIREi  = 49,    /*!< OrgaShooterBot shooting. */
  SOUND_GUNDEL  = 50,   /*!< Neutralized AlienInsect's fire under the shield. */
  SOUND_PSHHH2  = 51,   /*!< Shielder mechanic tower manipulating. */
  SOUND_MESSAGE  = 52,  /*!< DisplayMessage message. */
  SOUND_BOUMm  = 53,    /*!< Metal bang. */
  SOUND_BOUMv  = 54,    /*!< Plant bang. */
  SOUND_BOUMs  = 55,    /*!< Smooth bang. */
  SOUND_EXPLOl  = 56,   /*!< Bot explosion. */
  SOUND_EXPLOlp  = 57,  /*!< Power explosion. */
  SOUND_EXPLOp  = 58,   /*!< Bot/Power explosion. */
  SOUND_STEPh  = 59,    /*!< Step on a rock #1. */
  SOUND_STEPm  = 60,    /*!< Step on a rock #2. */
  SOUND_POWERON  = 61,  /*!< Power on. */
  SOUND_POWEROFF  = 62, /*!< Power off. */
  SOUND_AIE  = 63,      /*!< Astronaut pain "AIE!". */
  SOUND_WAYPOINT  = 64, /*!< WayPoint/Target2 activate/Flag placing. */
  SOUND_RECOVER  = 65,  /*!< Recycler sound. */ /* DUPLICATE OF SOUND_GUNDEL=50 */
  SOUND_DEADi  = 66,    /*!< AlienInsect dying scream. */
  SOUND_JOSTLE  = 67,   /*!< Leaf shaking. */
  SOUND_GFLAT  = 68,    /*!< FlatGround probing/Menu particle #8. */
  SOUND_DEADg  = 69,    /*!< Astronaut death "AU!". */
  SOUND_DEADw  = 70,    /*!< Astronaut drown "AHU!AHU!". */
  SOUND_FLYf  = 71,     /*!< Jet engine overheating. */
  SOUND_ALARMt  = 72,   /*!< Bot temperature alarm/Menu particle #9. */
  SOUND_FINDING  = 73,  /*!< Vault opening/ProxyActivating an object. */ /* DUPLICATE OF SOUND_WAYPOINT=64 */
  SOUND_THUMP  = 74,    /*!< Thumper thumping. */
  SOUND_TOUCH  = 75,    /*!< Bot getting damage. */
  SOUND_BLITZ  = 76,    /*!< Thunder. */
  SOUND_MUSHROOM  = 77, /*!< Mushroom poisoning. */
  SOUND_FIREp  = 78,    /*!< PhazerShooter shooting. */
  SOUND_EXPLOg1  = 79,  /*!< ShooterBots damaging objects. */
  SOUND_EXPLOg2  = 80,  /*!< OrgaShooterBots damaging objects. */
  SOUND_MOTORd  = 81,   /*!< Scribbler/ToyBot engine. */

  // BuzzingCars specific
SOUND_MOTOR1    = 105,
SOUND_MOTOR2    = 106,
SOUND_TAKE      = 129,
SOUND_WHEEL     = 141,   // roue avec pneu
SOUND_WHEELb    = 142,   // roue sur la jante
SOUND_HOME1     = 143,
SOUND_HORN      = 144,
SOUND_BREAK1    = 145,
SOUND_BREAK2    = 146,
SOUND_STARTREADY= 147,
SOUND_START3    = 148,
SOUND_START2    = 149,
SOUND_START1    = 150,
SOUND_STARTGO   = 151,
SOUND_SPRINKLE  = 152,
SOUND_RECORDgall= 153,   // record du circuit
SOUND_RECORDgone= 154,   // record du tour
SOUND_RECORDlone= 155,   // meilleur tour
SOUND_MOTOR3    = 157,
SOUND_MOTOR4    = 158,
SOUND_MOTOR5    = 159,
SOUND_MOTOR6    = 160,
SOUND_HELICO    = 162,
SOUND_LAP4      = 163,
SOUND_LAP3      = 164,
SOUND_LAP2      = 165,
SOUND_LAP1      = 166,
SOUND_BOT1c     = 167,
SOUND_BOT1p     = 168,
SOUND_BOT2c     = 169,
SOUND_BOT2p     = 170,
SOUND_BOT3c     = 171,
SOUND_BOT3p     = 172,
SOUND_BOT4c     = 173,
SOUND_BOT4p     = 174,
SOUND_EVIL1     = 175,
SOUND_WHEELg    = 176,   // roue sur gravier
SOUND_CHOCm     = 177,   // choc m�talique
SOUND_CHOCa     = 178,   // choc arbre
SOUND_FALLg     = 179,   // tombe dans le gravier
SOUND_CHOCo     = 180,   // choc objet
SOUND_FALLo1    = 181,   // objet tombe
SOUND_FALLo2    = 182,   // objet tombe
SOUND_FALLo3    = 183,   // objet tombe

  SOUND_MAX             /** number of items in enum */
};

SoundType ParseSoundType(const std::string& str);