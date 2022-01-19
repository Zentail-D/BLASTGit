ARTISTS:
Leave your assets in here as Blueprints and UAssets, there are two folders, one for UAssets and one for Blueprints. 
Please use them accordingly.


The assets .uasset should have set the proper
1. 	Collisions
2. 	LOD type
3. 	Appropriate Default material.
4.	Naming Convention

Mainly comes from  https://www.tomlooman.com/ue4-naming-convention/
with edits to shorten or cleanup convention

!!!!For asset naming convention!!!!!

Prefixes
Asset Type, Prefix, Example, Comment

Blueprint, BP_, BP_WallLight_01, Except for derived common classes: HUD / GameMode / Character
Blueprint Interface, BPI_, BPI_InventoryItem_01,
Material, M_, M_Grass_01,
Material Instance, MI_, MI_Grass_01,
Material Function, MF_, MF_CheapContrast, Not numbered
Material Parameter Collection, MPC_, MPC_EnvironmentSettings_01,
Static Mesh, SM_, SM_Wall_01,
Skeletal Mesh, SK_, SK_Character_01,
Skeletal Animation, SKa_Character_A_Idle
Texture, T_, T_Grass_01_D, Has suffix for texture types. See suffixes table.
Particle System, P_, P_Fire_01,
Physics Material, _PhysMat, Dirt_PhysMat, Not numbered
Sound, S_, S_HitImpact_Mono_01, Include _Mono or _Stereo designations
Sound Cue, S_*_Cue, S_HitImpact_01_Cue,
Attenuation, _att, Explosion_att,
Enumeration, E, EWeaponType, Not numbered. Similar to convention in code (enum EWeaponType)
Render Target, RT_, RT_CameraCapturePoint_01,
Vector/Float/Color Curve, Curve_, Curve_Recoil_AK47,
Camera Shake, CamShake_, CamShake_Landed,
User Widget, Widget_, Widget_EnergyBar,
Font, Font_, Font_Roboto48, Font size is included in name.


Suffixes

FOR ALL SUFFIXES!!!!!
Suffix Itteration Letter vs Number
If you have 3 versions of something name them Ex. SM_Wall_A, SM_Wall_B, SM_Wall_C.
DO NOT with numbers at the end as this sill become confusing once an asset is copied.
for assets that are imported these names should be named as such on import.


Textures Suffixes

Texture types all use the T_ prefix.

Texture type, Suffix

Diffuse/Color Map, _D
Normal Map, _N
Emissive Map, _E
Packed AO, Roughness, Metallic, Height Map, _ARMH
	*Ambient Occlusion, _AO
	*Roughness Map, _R
	*Metallic Map, _MT
	*Height Map, _H
---- Less common ----
Mask Map, _M (all mask maps should be 4 maps packed with suffix of _MP (masks packed) )
Specular, _S
Displacement,_DP
Flow Map, _F
Light Map (custom), _L


Animation Suffixes 
Animation Blueprint	_AnimBP
Physics Asset	_PhysicsAsset
Skeleton	_Skeleton
BlendSpace	_BlendSpace
AnimMontage	_Montage