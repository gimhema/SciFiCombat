SciFiCombat(작성중)
======================

![Hilight1](/Gifs/Hilight1.png)

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
Content


======================

# 3. Combat System
## 3.1 Input System
Content
## 3.2 Weapon System
### 3.2.1 Equip Weapon

### 3.2.2 Swap Weapon

### 3.2.3 Range Weapon
#### 3.2.3.1 Projectile Type Weapon
#### 3.2.3.2 Hitscan Type Weapon
#### 3.2.3.3 Sniper Rifle
#### 3.2.3.4 Shotgun
#### 3.2.3.5 Missile Launcher
#### 3.2.3.6 Grenade Launcher

### 3.2.4 Melee Weapon
#### 3.2.4.1 Sword
#### 3.2.4.2 Spear
#### 3.2.4.3 Axe

## 3.3 Item

## 3.4 Main Skill
### 3.4.1 Turret
### 3.4.2 Damage Immune
### 3.4.3 Barrier

## 3.5 Sub Skill
### 3.5.1 Impulse Grenade
### 3.5.2 Land Mine
### 3.5.3 Heal
### 3.5.4 Jump Booster
### 3.5.5 Speed Booster

Content



