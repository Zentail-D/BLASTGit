#pragma once

#include "Runtime/Core/Public/UObject/NameTypes.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"

namespace bb_keys
{
	TCHAR const* const target_location = TEXT("TargetLocation");
	TCHAR const* const can_see_player = TEXT("CanSeePlayer");
	TCHAR const* const player_is_in_melee_range = TEXT("PlayerIsInMeleeRange");
	TCHAR const* const is_investigating = TEXT("IsInvestigating");
	TCHAR const* const target_actor = TEXT("TargetActor");
	TCHAR const* const can_see_target = TEXT("CanSeeTarget?");
	TCHAR const* const move_to_location = TEXT("MoveToLocation");
	TCHAR const* const target = TEXT("Target");
	TCHAR const* const origin = TEXT("Origin");
	TCHAR const* const in_range = TEXT("Inrange");
	TCHAR const* const has_reached = TEXT("HasReached");
	TCHAR const* const need_path_refreshed = TEXT("NeedPathingRefreshed");
	
	
}
namespace tags
{
	TCHAR const* const noise_tag = TEXT("Noise");
}