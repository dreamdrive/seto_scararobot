# seto_scararobot について
 瀬戸内ROS勉強会のビーズセッター（スカラロボット）のROSパッケージです。

2019/12/05
とりあえず、rvizで表示されます。赤い…。

### 使い方

0. ROSワークスペースのsrcフォルダの中に設置して、catkin_make

1. 下記コマンドをたたく

```
$ roslaunch seto_scararobot display.launch model:=`(rospack find seto_scararobot)`/urdf/robo.urdf
```

2. rvizのGlobal OptionsのFixed Frameをbase_linkに変更
3. AddでRobotModelを追加

### 予定・課題
- DINAMIXEL Workbenchとの接続 (＆実機連携)
- MoveItとの連携
- エンドエフェクタのコントロールは要件等（本パッケージ未実装）
- 上位ノードについては要検討（従来のビーズセッターのプログラムとのゲートウェイノード）

