SciFiCombat
======================

![Hilight1](/Gifs/Hilight1.png)

# 0. Play Demo

link :

======================

# 1. Multi Play
## 1.1 Replication Datas

플레이어의 체력, 탄약, 장착중인 무기와 같이 네트워크상으로 공유가 필요한 정보들은 UPROPERTY(Replicated) 옵션을 통해 Replication 되어있습니다.

## 1.2 Unreal Remote Procedure Call(RPC)

- 플레이어가 원거리 무기를 발사하거나 근접 무기를 휘둘러 공격하는 등의 이벤트들은 다른 클라이언트들에게도 보여져야 하기때문에 RPC를 통해서 구현되었습니다.

- 마우스 좌측 클릭을 통해서 실행되는 공격함수는 플레이어가 장착중인 무기 종류에 따라 원거리 무기를 발사할지, 근접 무기를 휘두를지를 결정합니다. 이 때 이를 처리하는 함수는 내부적으로 UFUNCTION(Server) 키워드로 선언된 함수들을 호출합니다.

- Server 키워드로 호출된 함수들은 내부적으로 Multicast 함수들을 호출합니다.

- Multicast 키워드로 구현된 RPC 함수는 서버에서 호출될 경우 서버와 모든 클라이언트에서 실행되기때문에 모든 플레이어들은 자신이 어떤방향으로 총을 쏘고 있는지, 검을 휘두르는지를 다른 유저들에게 보여줄 수 있습니다.

- 스킬 또한 동일한 방식으로 동작합니다.


======================

# 2. UI
## 2.1 Game Start

![StartUI](/Gifs/CharacterSelectUI.PNG)
캐릭터 이미지 하단의 Select 버튼을 눌러 외형과 Main Skill을 선택할 수 있습니다.

## 2.2 HUD

![HUDUI](/Gifs/MainHUD_n.PNG)

- (1번) 체력바를 나타냅니다. 체력이 0이되면 캐릭터는 사망하며 잠시 후 시작지점에서 부활하게됩니다.

- (2번) 근접무기 스매시 공격을 위한 게이지입니다. 게이지가 모두 채워져야 스매시 공격을 할 수 있습니다. 충분한 게이지가 없다면 아래와 같은 경고 메세지가 뜨게됩니다.
![EnoughSmashPowerProcess](/Gifs/EnoughSmashPowerProcess.gif)

- (3번) 메인 스킬과 서브 스킬을 사용하기위한 마나입니다. 마나는 몬스터를 처치하여 얻을 수 있습니다. 충분한 마나가 없다면 아래와 같은 경고 메세지가 뜨게됩니다.
- ![EnoughManaProcess](/Gifs/EnoughManaProcess.gif)

- (4번) 메인 스킬입니다. 시작화면에서 선택한 캐릭터에 따라서 바뀌게됩니다.

- (5번) 서브 스킬입니다. B키를 눌러 선택할 수 있습니다.
![SubskillSelect](/Gifs/SubskillSelect.gif)
![SelectSubAbility](/Gifs/SelectSubAbility.gif)


- (6번) 원거리 무기를 장착했을 경우 탄창을 표시하며 근접무기를 장착했을 경우 Meele Weapon 이라는 이미지를 띄웁니다. 하단에는 장착한 무기의 이미지를 띄워줍니다.

- (7번) 게임 플레이 제한 시간을 나타냅니다. 정해진 시간에 도달할 경우 게임이 다시 시작됩니다.



======================

# 3. Combat System
## 3.1 Input System


![InputSystem](/Gifs/InputSystem.PNG)


### 3.1.1 Movement

- WASD 키를 통해서 이동이 가능합니다.

- Shift키를 누르고있으면 Crouch 모드가 활성화되어 이동합니다. 활성화 중에는 Capsule Component의 크기와 이동속도가 줄어들고 Crouch 자세로 이동합니다. Anim Blend 1D와 Anim Blueprint의 Anim Graph를 사용하여 구현했습니다.

- 스페이스 바를 누르면 점프를 할 수 있습니다. 점프중에는 한번 더 점프를 할 수 있습니다. 더블 점프는 LaunchCharacter 함수를 Z 방향으로 적용하여 구현했습니다.

- F키를 누르면 회피를 할 수 있습니다. 회피에 해당하는 애니메이션 시퀀스를 실행하며 캐릭터의 전방 방향으로 Force를 적용하여 튀어 나가듯이 이동합니다.


## 3.2 Weapon System

### 3.2.1 Equip Weapon

![EquipWeapon](/Gifs/EquipWeapon.gif)
![EquipWeapon](/Gifs/EquipSubWeapon.gif)

- 무기 근처에 다가가면 E 키를 눌러 무기를 장착을 할 수 있다는 안내 Widget이 팝업됩니다.

- 무기 근처에서 E 키를 누르면 무기를 장착할 수 있습니다.

- 만약 플레이어가 이미 무기를 들고있다면 등 부분의 Sub Weapon Socket에 무기를 장착하게 됩니다.

- 원거리 무기의 경우 기존에 남아있는 Ammo들이 그대로 가진 상태로 Drop됩니다.(초기화되지않음)

### 3.2.2 Swap Weapon

![SwapHitScanWeaponFire](/Gifs/SwapHitScanWeaponFire.gif)

- 모든 무기들은 EWeaponStyle 열거형 타입을 가지고 있으며, 이 타입을 통해 원거리 무기인지 근접 무기인지 정해집니다.

- 정해진 무기 타입에 따라서 Animation Blueprint에서 취해야할 자세들이 바뀝니다.

- (보조무기와 주무기를 모두 장착한 상태) 무기 근처에서 E 키를 누르게되면 들고있던 주무기는 Drop되며 교체됩니다.

- (보조무기와 주무기를 모두 장착한 상태) 무기 밖에서 E 키를 누르게되면 무기 교체를 실시합니다. 


### 3.2.3 Range Weapon

#### 3.2.3.1 Projectile Type Weapon

![ProjectileWeaponFire](/Gifs/ProjectileWeaponFire.gif)

- 카메라에서 LineTrace를 발사한 뒤 발생하는 HitPoint로부터 Projectile이 도달해야할 Location을 계산합니다.

- 총의 Muzzle 소켓의 Location을 구하고, 이 Location과 이전 단계에서 계산한 Projectile의 도달위치 간 Vector를 구합니다.

- 이 Vector로부터 Rotation을 구하여 Projectile의 발사 각도를 설정합니다.

- Projectile의 발사 각도를 구하고나면 Projectile Actor를 Spawn합니다. Projectile은 Projectile - Movement Component가 부착되어 있기 때문에 지정된 속도로 발사됩니다.

- Projectile은 Hit Event가 실행되면 지정해둔 OnHit 함수에 따라 파괴되거나, 몬스터에게 데미지를 입힙니다.

#### 3.2.3.2 Hitscan Type Weapon

![SwapHitScanWeaponFire](/Gifs/SwapHitScanWeaponFire.gif)

- 카메라에서 발사된 LineTrace를 발사하고 FHitResult로부터 GetActor()함수를 사용하여 이를 Monster로 Cast를 진행한 뒤 유효한 경우 Hit된 몬스터에게 데미지를 적용합니다.

#### 3.2.3.3 Sniper Rifle

![SniperWeaponFire](/Gifs/SniperWeaponFire.gif)

- 데미지를 입히는 방식은 HitScan과 동일합니다.

- 우클릭으로 조준할경우 UMG 애니메이션이 실행됩니다.

#### 3.2.3.4 Shotgun

![SpreadWeaponFire](/Gifs/SpreadWeaponFire.gif)

- 데미지를 입히는 방식은 HitScan과 동일합니다.

- 발사 위치의 Vector에 Random하게 생성되는 Vector를 더하고, Shotgun Bullet의 개수만큼 반복문을 돌려서 발사를 진행합니다.


#### 3.2.3.5 Missile Launcher

![MissileWeaponFire](/Gifs/MissileWeaponFire.gif)

- 발사체형 무기와 같은 발사 방식을 가지고 있습니다.

- Hit 이벤트가 발생하면 광역 데미지 이벤트를 발생시킵니다. 범위내에 있는 몬스터, 플레이어는 데미지를 입습니다.


#### 3.2.3.6 Grenade Launcher

![GrenadeWeaponFire](/Gifs/GrenadeWeaponFire.gif)

- 로켓 런쳐와 유사한 방식으로 동작하지만 발사체에 Bound의 옵션을 통해 유탄과 같은 효과를 연출합니다.


### 3.2.4 Melee Weapon

- 무기에는 HitBox가 부착되어 있으며 공격 이벤트가 실행될때마다 HitBox가 활성화됩니다.

- HitBox가 활성화 되어 있을 때 몬스터와 Overlap되면 지정해둔 Overlap 함수가 실행되어 몬스터에게 데미지를 입힙니다.

- 몬스터가 근접무기를 통해서 데미지를 입을때는 지정된 위치에서 피격 이펙트와 사운드를 재생합니다.

- 근거리무기는 공격이 적중할때마다 Smash 게이지를 모을 수 있습니다. Smash 게이지를 모두 모으면 스매시 공격을 사용할 수 있습니다.

- Axe와 Spear는 공격 시 자체경직이 적용됩니다. (움직일 수 없음)

======================

#### 3.2.4.1 Sword

![SwordSwing](/Gifs/SwordSwing.gif)
![SwordHitGetManaPack](/Gifs/SwordHitGetManaPack.gif)
![SwordSmash](/Gifs/SwordSmash.gif)

======================

#### 3.2.4.2 Spear

![CombatSpearAttack1_n](/Gifs/CombatSpearAttack1_n.gif)
![CombatSpearAttack2_n](/Gifs/CombatSpearAttack2_n.gif)
![CombatSpearAttack3_n](/Gifs/CombatSpearAttack3_n.gif)
![CombatSpearHit1](/Gifs/CombatSpearHit1.gif)
![CombatSpearSmash1](/Gifs/CombatSpearSmash1.gif)
![CombatSpearSmash2](/Gifs/CombatSpearSmash2.gif)

======================

#### 3.2.4.3 Axe

![AxeSwing_n](/Gifs/AxeSwing_n.gif)
![AxeHit1](/Gifs/AxeHit1.gif)
![AxeHit2](/Gifs/AxeHit2.gif)
![AxeSmashSwing](/Gifs/AxeSmashSwing.gif)
![AxeSmashHit](/Gifs/AxeSmashHit.gif)

- Axe는 스매시 공격을 적중시키면 적들을 공중에 띄을 수 있습니다.

======================

## 3.3 Item

![GetAmmoItem](/Gifs/GetAmmoItem.gif)

- AmmoItem은 Sphere Collision으로 설정되어있는 PickUP Area를 가지고 있습니다. Player가 이 Area에 Overlap하면 AmmoItem의 구성정보(무기 타입, 탄약 수)에 따라 Player의 Ammo Status가 업데이트됩니다.

## 3.4 Main Skill

### 3.4.1 Turret

![InstallTurret1](/Gifs/InstallTurret1.gif)
![InstallTurret2](/Gifs/InstallTurret2.gif)

- Sensing Component를 통해 전방에 감지되는 몬스터가 있을 경우 해당 몬스터 방향으로 몸을 틀어 공격하는 터렛을 소환합니다. 이 터렛은 Behaivior Tree를 기반으로 동작합니다.


### 3.4.2 Damage Immune

![DamageImmuneCasting](/Gifs/DamageImmuneCasting.gif)
![DamageImmune](/Gifs/DamageImmune.gif)

- 일시적으로 데미지를 받지않는 상태가 됩니다.


### 3.4.3 Barrier

![BarrierCasting](/Gifs/BarrierCasting.gif)
![Barrier_n](/Gifs/Barrier_n.gif)

-적의 투사체를 파괴하는 벽을 생성합니다.

## 3.5 Sub Skill

### 3.5.1 Impulse Grenade

![ImpulseSkill](/Gifs/ImpulseSkill.gif)

- 전방에 Radial Impulse를 발생시키는 수류탄을 투척합니다.

### 3.5.2 Land Mine

![InstallMine](/Gifs/InstallMine.gif)
![MineExplosion](/Gifs/MineExplosion.gif)

- 캐릭터 전방에 몬스터가 Overlap되면 데미지를 입히는 지뢰를 설치합니다.


### 3.5.3 Heal

![Heal](/Gifs/Heal.gif)

- 사용시 플레이어와 플레이어 주변의 유저들의 체력을 회복시킵니다.

### 3.5.4 Jump Booster

![JumpUp1](/Gifs/JumpUp1.gif)
![JumpUp2_n](/Gifs/JumpUp2_n.gif)

- 플레이어의 Jump Z Value를 일시적으로 증가시켜 더 높이 점프할 수 있도록 합니다.

### 3.5.5 Speed Booster

![SpeedUp_n](/Gifs/SpeedUp_n.gif)

- 플레이어의 Max Walk Speed를 일시적으로 증가시켜 더 빠른 속도로 이동합니다.


