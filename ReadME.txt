敵を倒して最奥を目指すゲーム

-プレイ動画-
https://drive.google.com/file/d/1mCfJoSJApa9zvhR2U6iR5XaEOvRHJkYE/view?usp=drive_link

-操作方法-
WASD移動
左・右クリック攻撃
マウスホイールでサブ武器の選択
Eキーアクション（拾う・ワープの使用など
Qキーターゲット（敵をターゲット状態に

-アピールポイント-
プレイヤーの操作感
敵の行動AI

-フォルダ構成-
D:.
│  AnimationController.cpp
│  AnimationController.h
│  AudioManager.cpp
│  AudioManager.h
│  Deeper.sln
│  Deeper.vcxproj
│  Deeper.vcxproj.filters
│  Deeper.vcxproj.user
│  GameBaseDx11.vcxproj.user
│  VFXManager.cpp
│  VFXManager.h
│
├─Action
│      BaseAction.cpp
│      BaseAction.h
│      MoveAction.cpp
│      MoveAction.h
│      RotateAction.cpp
│      RotateAction.h
│      SearchAction.cpp
│      SearchAction.h
│
├─BehaviorTree
│      BehaviourNode.cpp
│      BehaviourNode.h
│      ChangeStateNode.cpp
│      ChangeStateNode.h
│      IsEnemyActionReadyNode.cpp
│      IsEnemyActionReadyNode.h
│      IsEnemyPermission.cpp
│      IsEnemyPermission.h
│      IsEnemyStateNode.cpp
│      IsEnemyStateNode.h
│      PlayerConditionNode.cpp
│      PlayerConditionNode.h
│      StateCountNode.cpp
│      StateCountNode.h
│
├─Character
│      Character.cpp
│      Character.h
│      CharacterManager.cpp
│      CharacterManager.h
│
├─Enemy
│      DropTable.cpp
│      DropTable.h
│      EnemyBase.cpp
│      EnemyBase.h
│      EnemyManager.cpp
│      EnemyManager.h
│      EnemyUi.cpp
│      EnemyUi.h
│      MeleeFighter.cpp
│      MeleeFighter.h
│      StoneGolem.cpp
│      StoneGolem.h
│      ThrowEnemy.cpp
│      ThrowEnemy.h
│
├─Engine
│      Audio.cpp
│      Audio.h
│      BillBoard.cpp
│      BillBoard.h
│      BoxCollider.cpp
│      BoxCollider.h
│      Camera.cpp
│      Camera.h
│      Collider.cpp
│      Collider.h
│      CsvReader.cpp
│      CsvReader.h
│      Debug.cpp
│      Debug.h
│      Direct3D.cpp
│      Direct3D.h
│      Easing.cpp
│      Easing.h
│      Fbx.cpp
│      Fbx.h
│      FbxParts.cpp
│      FbxParts.h
│      GameObject.cpp
│      GameObject.h
│      Global.h
│      Image.cpp
│      Image.h
│      Input.cpp
│      Input.h
│      Main.cpp
│      Model.cpp
│      Model.h
│      PolyLine.cpp
│      PolyLine.h
│      RootObject.cpp
│      RootObject.h
│      SceneManager.cpp
│      SceneManager.h
│      SegmentCollider.cpp
│      SegmentCollider.h
│      SphereCollider.cpp
│      SphereCollider.h
│      Sprite.cpp
│      Sprite.h
│      Text.cpp
│      Text.h
│      Texture.cpp
│      Texture.h
│      Transform.cpp
│      Transform.h
│      TransitionEffect.cpp
│      TransitionEffect.h
│      VFX.cpp
│      VFX.h
│
├─GameManager
│      CombatAI.cpp
│      CombatAI.h
│      DifficultyManager.cpp
│      DifficultyManager.h
│      GameManager.cpp
│      GameManager.h
│      WaveManager.cpp
│      WaveManager.h
│
├─Ornament
│      PlayerOrnament.cpp
│      PlayerOrnament.h
│      WeaponMainOrnament.cpp
│      WeaponMainOrnament.h
│
├─Player
│      Aim.cpp
│      Aim.h
│      LifeManager.cpp
│      LifeManager.h
│      Player.cpp
│      Player.h
│      PlayerCommand.cpp
│      PlayerCommand.h
│      PlayerData.cpp
│      PlayerData.h
│      PlayerWeapon.cpp
│      PlayerWeapon.h
│
├─Scene
│      ResultScene.cpp
│      ResultScene.h
│      SceneBase.cpp
│      SceneBase.h
│      Stage1.cpp
│      Stage1.h
│      Stage2.cpp
│      Stage2.h
│      Stage3.cpp
│      Stage3.h
│      StageBase.cpp
│      StageBase.h
│      TitleScene.cpp
│      TitleScene.h
│
├─Stage
│      Cell.cpp
│      Cell.h
│      CollisionMap.cpp
│      CollisionMap.h
│      CreateStage.cpp
│      CreateStage.h
│      NavigationAI.cpp
│      NavigationAI.h
│      SkyBox.cpp
│      SkyBox.h
│      Triangle.cpp
│      Triangle.h
│      Warp.cpp
│      Warp.h
│
├─State
│      MeleeFighterState.cpp
│      MeleeFighterState.h
│      PlayerState.cpp
│      PlayerState.h
│      StateBase.h
│      StateManager.cpp
│      StateManager.h
│      StoneGolemState.cpp
│      StoneGolemState.h
│      ThrowState.cpp
│      ThrowState.h
│
├─UI
│      ExitUIManager.cpp
│      ExitUIManager.h
│      Interaction.cpp
│      Interaction.h
│      InteractionUI.cpp
│      InteractionUI.h
│      ResultUIManager.cpp
│      ResultUIManager.h
│      TitleUIManager.cpp
│      TitleUIManager.h
│      UIBase.cpp
│      UIBase.h
│      UIManager.cpp
│      UIManager.h
│
└─Weapon
        BulletBase.cpp
        BulletBase.h
        MainSwordWeapon.cpp
        MainSwordWeapon.h
        NormalBullet.cpp
        NormalBullet.h
        NormalBulletWeapon.cpp
        NormalBulletWeapon.h
        StoneArmWeapon.cpp
        StoneArmWeapon.h
        ThrowBullet.cpp
        ThrowBullet.h
        WeaponBase.cpp
        WeaponBase.h
        WeaponObject.cpp
        WeaponObject.h
        WeaponObjectManager.cpp
        WeaponObjectManager.h