# seto_scararobot について
 瀬戸内ROS勉強会のビーズセッター（スカラロボット）のROSパッケージです。

### 使い方

1. ROSワークスペースのsrcフォルダの中にクローンして、catkin_make

2. 下記コマンドをたたく

```
$ roslaunch seto_scararobot display.launch model:=`(rospack find seto_scararobot)`/urdf/robo.urdf
```

### 予定・課題
- DINAMIXEL Workbenchとの接続 (＆実機連携)
- MoveItとの連携
- エンドエフェクタのコントロールは要検討（本パッケージ未実装）
- 上位ノードについては要検討（従来のビーズセッターのプログラムとのゲートウェイノード）

### 履歴
- 2019/12/05 : とりあえず、rvizで表示されます。赤い…。
- 2019/12/08 : kawataさんの修正をマージして、色の問題は修正済み。
