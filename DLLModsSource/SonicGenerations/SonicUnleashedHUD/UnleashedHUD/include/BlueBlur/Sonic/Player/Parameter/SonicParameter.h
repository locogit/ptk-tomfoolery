﻿#pragma once

namespace Sonic::Player
{
    enum ESonicParameter
    {
        eSonicParameter_AdditinalMaxVelocityLv1 = 478,
        eSonicParameter_AdditinalMaxVelocityLv2 = 479,
        eSonicParameter_AdditinalMaxVelocityLv3 = 480,
        eSonicParameter_AdditinalMaxVelocityLv4 = 481,
        eSonicParameter_AdditinalMaxVelocityLv5 = 482,
        eSonicParameter_AdditinalMaxVelocityLv6 = 483,
        eSonicParameter_AdditinalMaxVelocityLv7 = 484,
        eSonicParameter_AdditinalMaxVelocityLv8 = 485,
        eSonicParameter_AdditinalMaxVelocityLv9 = 486,
        eSonicParameter_AdditinalMaxVelocityLv10 = 487,
        eSonicParameter_AdditinalMaxVelocityBySkill = 488,
        eSonicParameter_BorderTimeOfBrakeCancelForStartDrift = 489,
        eSonicParameter_StartDriftVelocityBorder = 490,
        eSonicParameter_StartDriftInputAngleBorder = 491,
        eSonicParameter_StartDriftFromSlidingBorder = 492,
        eSonicParameter_StompingDownFirstVelocity = 496,
        eSonicParameter_StompingAirDrag = 497,
        eSonicParameter_StompingMinVertVeloicty = 498,
        eSonicParameter_StompingDisableTime = 499,
        eSonicParameter_StompingHorzDecForce = 500,
        eSonicParameter_StompingAccel = 501,
        eSonicParameter_StompingMaxVelocity = 502,
        eSonicParameter_StompingStartAccel = 503,
        eSonicParameter_IsLightSpeedDashStartCollisionEdit = 504,
        eSonicParameter_IsLightSpeedDashNaviCollisionEdit = 505,
        eSonicParameter_IsLightSpeedDashCollisionEdit = 506,
        eSonicParameter_LightSpeedDashStartCollisionAspect = 507,
        eSonicParameter_LightSpeedDashStartCollisionFovy = 508,
        eSonicParameter_LightSpeedDashStartCollisionNear = 509,
        eSonicParameter_LightSpeedDashStartCollisionFar = 510,
        eSonicParameter_LightSpeedDashStartCollisionRotationX = 511,
        eSonicParameter_LightSpeedDashStartCollisionOffsetX = 512,
        eSonicParameter_LightSpeedDashStartCollisionOffsetY = 513,
        eSonicParameter_LightSpeedDashNaviCollisionAspect = 514,
        eSonicParameter_LightSpeedDashNaviCollisionFovy = 515,
        eSonicParameter_LightSpeedDashNaviCollisionNear = 516,
        eSonicParameter_LightSpeedDashNaviCollisionFar = 517,
        eSonicParameter_LightSpeedDashNaviCollisionRotationX = 518,
        eSonicParameter_LightSpeedDashNaviCollisionOffsetX = 519,
        eSonicParameter_LightSpeedDashNaviCollisionOffsetY = 520,
        eSonicParameter_LightSpeedDashCollisionAspect = 521,
        eSonicParameter_LightSpeedDashCollisionFovy = 522,
        eSonicParameter_LightSpeedDashCollisionNear = 523,
        eSonicParameter_LightSpeedDashCollisionFar = 524,
        eSonicParameter_LightSpeedDashCollisionRotationX = 525,
        eSonicParameter_LightSpeedDashCollisionOffsetX = 526,
        eSonicParameter_LightSpeedDashCollisionOffsetY = 527,
        eSonicParameter_LightSpeedDashMinVelocity = 528,
        eSonicParameter_LightSpeedDashMaxVelocity = 529,
        eSonicParameter_LightSpeedDashMinVelocity3D = 530,
        eSonicParameter_LightSpeedDashMaxVelocity3D = 531,
        eSonicParameter_LightSpeedDashHoldTime = 532,
        eSonicParameter_LightSpeedDashEnableAngle = 533,
        eSonicParameter_IsLightSpeedDashHoldMode = 534,
        eSonicParameter_GrindDamageTime = 536,
        eSonicParameter_GrindDamageVelocityDecreaseRate = 537,
        eSonicParameter_GrindSideJumpForce = 538,
        eSonicParameter_GrindCurveDecreaseForce = 539,
        eSonicParameter_GrindStumbleLimit = 540,
        eSonicParameter_GrindStumbleLimitTime = 541,
        eSonicParameter_GrindToWallWalkFrontVelocity = 542,
        eSonicParameter_GrindToWallWalkSideVelocity = 543,
        eSonicParameter_GrindToWallWalkVertVelocity = 544,
        eSonicParameter_StompingStrength = 547,
        eSonicParameter_DriftLowSpeedVelocityThreshold = 548,
        eSonicParameter_DriftAngleReturnForce = 549,
        eSonicParameter_DriftAngleReturnForceLowSpeed = 550,
        eSonicParameter_DriftSideAngleForce = 551,
        eSonicParameter_DriftSideAngleForceLowSpeed = 552,
        eSonicParameter_DriftFrontAngleForce = 553,
        eSonicParameter_DriftFrontAngleForceLowSpeed = 554,
        eSonicParameter_DriftMaxAngleVelocity = 555,
        eSonicParameter_DriftMaxAngleVelocityLowSpeed = 556,
        eSonicParameter_DriftSideDecForce = 557,
        eSonicParameter_DriftSideDecForceLowSpeed = 558,
        eSonicParameter_DriftFrontAccelForceRate = 559,
        eSonicParameter_DriftFrontAccelForceRateLowSpeed = 560,
        eSonicParameter_DriftFinishAngle = 561,
        eSonicParameter_DriftFinishSideVelocity = 562,
        eSonicParameter_DriftFinishVelocity = 563,
        eSonicParameter_DriftBoostYawOffset = 564,
        eSonicParameter_DriftFailureAddVelocity = 565,
        eSonicParameter_DriftSuccessAddVelocity = 566,
        eSonicParameter_DriftDecVelocityFirst = 567,
        eSonicParameter_DriftDecVelocityFirstLowSpeed = 568,
        eSonicParameter_EnemyBlockedMaxSquatVelocity = 569,
        eSonicParameter_DashModePathCorrectSideSpeed = 572,
        eSonicParameter_DashModePathCorrectSideSpeedDigital = 573,
        eSonicParameter_DivingRotationMax = 574,
        eSonicParameter_DivingDiveBeginHorzVelocityRate = 575,
        eSonicParameter_DivingDiveBeginGravityScale = 576,
        eSonicParameter_DivingDiveMaxVelocityDown = 577,
        eSonicParameter_DivingDiveVelocitySide = 578,
        eSonicParameter_DivingDiveVelocityFront = 579,
        eSonicParameter_DivingDiveEndSpeed = 580,
        eSonicParameter_DivingFloatMaxVelocityDown = 581,
        eSonicParameter_DivingFloatVelocitySide = 582,
        eSonicParameter_DivingFloatVelocityFront = 583,
        eSonicParameter_DivingDamageVelocityDecreaseRate = 584,
        eSonicParameter_DivingDamageTime = 585,
        eSonicParameter_ItemCollisionHeightInDiving = 586,
        eSonicParameter_ItemCollisionRadiusInDiving = 587,
        eSonicParameter_HurdleJumpRotateZRate = 588,
        eSonicParameter_HurdleFirstAnimSpeed = 589,
        eSonicParameter_HurdleReleaseAnimSpeed = 590,
        eSonicParameter_HurdleFlyingTime = 591,
        eSonicParameter_HurdleJumpVelocity = 592,
        eSonicParameter_HurdleJumpLimitHeight = 593,
        eSonicParameter_HurdleFallVelocity = 594,
        eSonicParameter_MotionStandToWalk = 595,
        eSonicParameter_MotionWalkToWalkFast = 596,
        eSonicParameter_MotionWalkFastToJog = 597,
        eSonicParameter_MotionJogToRun = 598,
        eSonicParameter_MotionRunToDash = 599,
        eSonicParameter_MotionDashToJet = 600,
        eSonicParameter_MotionJetToBoost = 601,
        eSonicParameter_SquatKickInputTime = 602,
        eSonicParameter_EnemyComboTime = 603,
        eSonicParameter_WindEffectCount = 604,
        eSonicParameter_WindEffectMeshPointCount = 605,
        eSonicParameter_WindEffectRandAmplitude = 606,
        eSonicParameter_WindEffectRandFrequency = 607,
        eSonicParameter_WindEffectDirOffset = 608,
        eSonicParameter_WindEffectSpeedMin = 609,
        eSonicParameter_WindEffectSpeedMax = 610,
        eSonicParameter_WindEffectWidth = 611,
        eSonicParameter_WindEffectWidthRand = 612,
        eSonicParameter_WindEffectLife = 613,
        eSonicParameter_WindEffectLifeRand = 614,
        eSonicParameter_WindEffectIntervalTime = 615,
        eSonicParameter_WindEffectStartPosRange = 616,
        eSonicParameter_SparkleEffectCount = 617,
        eSonicParameter_SparkleEffectLife = 618,
        eSonicParameter_SparkleEffectLifeRand = 619,
        eSonicParameter_SparkleEffectWidth = 620,
        eSonicParameter_SparkleEffectWidthRand = 621,
        eSonicParameter_SparkleEffectSpeedMin = 622,
        eSonicParameter_SparkleEffectSpeedMax = 623,
        eSonicParameter_SparkleEffectMeshPointCount = 624,
        eSonicParameter_SparkleEffectRandDir = 625,
        eSonicParameter_SparkleEffectRandSpeed = 626,
        eSonicParameter_SparkleEffectIntervalTime = 627,
        eSonicParameter_BoardDamageOutOfControlTime = 628,
        eSonicParameter_BoardDamageVelocity = 629,
        eSonicParameter_BoardDamageInternalScale = 630,
        eSonicParameter_RocketStartNoControlTime = 631,
        eSonicParameter_RocketStartSpdKeepTime = 632,
        eSonicParameter_RocketStartAddVelocity = 633,
        eSonicParameter_SpsCameraRatio = 634,
        eSonicParameter_SpsCameraOffsY = 635,
        eSonicParameter_SpsCameraOffsZ = 636,
        eSonicParameter_SpsCameraTgtRatio = 637,
        eSonicParameter_SpsCameraTgtOffsY = 638,
        eSonicParameter_SpsCameraTgtOffsZ = 639,
        eSonicParameter_SpsCameraFollowRatio = 640,
        eSonicParameter_SpsCameraFovy = 641,
    };
}