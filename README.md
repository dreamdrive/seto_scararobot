# seto_scararobot について

## 使い方

### ビルド

1. ROSワークスペースのsrcフォルダの中にクローンして、catkin_make

```
$ cd ~/catkin_ws/src
$ git clone https://github.com/dreamdrive/seto_scararobot.git
$ cd ..
$ catkin_make
```

### 使い方1

1. 下記コマンドをたたく(joint_state_publisherでスライダを動かして遊べます)

```
$ roslaunch seto_scararobot display.launch
```

### 使い方2

1. 下記コマンドをたたく(勝手に動きます)

```
$ roslaunch seto_scararobot display_move_test.launch
```
joint_state_publisherの代わりに、move_testノードが立ち上がります。

### 使い方3 (IKで動かす)

1. 下記コマンドをたたく

```
$ roslaunch seto_scararobot display_move_arm.launch
```

2. 座標は別ターミナルで、
```
rostopic pub /beads_position geometry_msgs/Point　
```
と入力してTABを押せば座標を入力＆送信できます。(x,yはmm zは入力しても無効)

### 使い方4 (MoveItのデモでグリグリしてrvizで遊ぶ)

1. 下記コマンドをたたく

```
$ roslaunch seto_scararobot_moveit_config demo.launch 
```

### 使い方5 (実機の状態を読み込んで、rvizのモデルに反映させる)

1. 下記コマンドをたたく

```
$ roslaunch seto_scararobot_dynamixel dynamixel_controllers_twin.launch 
$ roslaunch seto_scararobot display_standalone.launch
```

### 使い方6 (MoveItのデモを実機に連動させて動作させる)

1. 下記コマンドをたたく

```
$ roslaunch seto_scararobot_dynamixel dynamixel_controllers.launch 
$ roslaunch seto_scararobot_moveit_config demo.launch
$ rosrun seto_scararobot moveit2dynamixel2
```

## 予定・課題
- DINAMIXEL Workbenchとの接続 (＆実機連携) 実機完成後
- MoveItとの連携
- エンドエフェクタのコントロールは要検討（本パッケージ未実装）
- 上位ノードについては要検討（従来のビーズセッターのプログラムとのゲートウェイノード）
- MoveItで、座標指定して動かすソースが動いていない(moveit_test.cpp,test2.py,どちらも。誰かhelp!)

## 履歴
- 2019/12/05 :  とりあえず、rvizで表示されます。赤い…。
- 2019/12/08 :  kawataさんの修正をマージして、色の問題は修正済み。
- 2019/12/10 :  move_testを作ってみた
- 2019/12/15 :  kawataさんのmove_armを合体
- 2019/12/21 :  MoveItで動かせるようにしてみた。／ディレクトリ構造を修正しています。／STLファイルの軽量化／目標点に架空のジョイント追加
- 2020/01/26 :  Dynamixel Workbench Controllersで、アームのサーボと通信可能に
                実機に合わせてurdfのjointの回転方向を整頓

