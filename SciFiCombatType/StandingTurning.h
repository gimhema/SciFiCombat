#pragma once

UENUM(BlueprintType)
enum class ETurningStanding : uint8
{
	ETURN_STANDING_LEFT UMETA(DisplayName = "Turn Left"),
	ETURN_STANDING_RIGHT UMETA(DisplayName = "Turn Right"),
	ETURN_STANDING_IDLE UMETA(DisplayName = "IDLE"),
	ETURN_MAX UMETA(DisplayName = "MAX")
};