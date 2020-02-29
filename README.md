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

### 使い方1 (joint_state_publisherでスカラロボットをスライダを動かせます。)

1. 下記コマンドを実行

```
$ roslaunch seto_scararobot_description display.launch
```

### 使い方2 (スカラロボットが勝手に動きます。)

1. 下記コマンドを実行

```
$ roslaunch seto_scararobot move_test_rviz.launch
```
joint_state_publisherの代わりに、move_testノードが立ち上がります。

### 使い方3 (IKで座標を指定して動かす。)

1. 下記コマンドを実行

```
$ roslaunch seto_scararobot move_arm_rviz.launch
```
rqt_ez_publisherが立ち上がりますので、x,yのスライダを動かすことでスカラロボットが動きます。単位は(mm)です。
※ rqt_ez_publisherパッケージをあらかじめインストールしておく必要があります。



### 使い方4 (MoveItのデモをグリグリ動かす)

1. 下記コマンドを実行

```
$ roslaunch seto_scararobot_moveit_config demo.launch 
```

### 使い方5 (実機の状態を読み込んで、rvizのモデルに反映させる)

1. 下記コマンドを実行

```
$ roslaunch seto_scararobot_dynamixel dynamixel_controllers_rviz.launch 
$ roslaunch seto_scararobot_description display_no_pub_jointstate.launch
```

### 使い方6 (MoveItのデモを実機に連動させて動作させる。 ※実機が必要)

1. 下記コマンドを実行

```
$ roslaunch seto_scararobot_dynamixel dynamixel_controllers.launch 
$ roslaunch seto_scararobot_moveit_config demo.launch
$ rosrun seto_scararobot moveit2dynamixel2
```

### 使い方7 (ロボットアームを動かす&RVIZで確認する。 ※実機が必要)

1. 下記コマンドを実行

```
$ roslaunch seto_scararobot_dynamixel dynamixel_controllers_rviz.launch
$ roslaunch seto_scararobot move_arm_hardware.launch
```
2. 座標は別ターミナルで、
```
rostopic pub /beads_position geometry_msgs/Point　
```
と入力してTABを押せば座標を実行できます。(x,yはmm zは入力しても無効)

## 予定・課題
* DINAMIXEL Workbenchとの接続 (＆実機連携) 実機完成後
* MoveItとの連携
* エンドエフェクタのコントロールは要検討（本パッケージ未実装）
* 上位ノードについては要検討（従来のビーズセッターのプログラムとのゲートウェイノード）
* MoveItで、座標指定して動かすソースが動いていない(moveit_test.cpp,test2.py,どちらも。誰かhelp!)
* 実機を動かすときは、FTDIのレイテンシタイマに注意 ( https://woodencaliper.hatenablog.com/entry/2018/06/30/172643 )

## 履歴
* 2019/12/05 :  とりあえず、rvizで表示されます。赤い…。
* 2019/12/08 :  kawataさんの修正をマージして、色の問題は修正済み。
* 2019/12/10 :  move_testを作ってみた
* 2019/12/15 :  kawataさんのmove_armを合体
* 2019/12/21 :  MoveItで動かせるようにしてみた。
  * ディレクトリ構造を修正しています。
  * STLファイルの軽量化
  * 目標点に架空のジョイント追加
* 2020/01/26 :  Dynamixel Workbench Controllersで、アームのサーボと通信可能にした。
  * 実機に合わせてurdfのjointの回転方向を整頓
* 2020/02/20 :  先日のもくもく会の進捗を反映＆使い方7を追加
* 2020/02/29 :  データ整理（下記の通り）
  * seto_scararobot_descriptionパッケージを作成し、urdfとmeshはこちらに移動
  * seto_scararobotパッケージからurdfとmeshを削除
  * urdfに慣性モーメントのパラメータを追記
  * display.launch をseto_scararobot_descriptionパッケージへ移動
  * display_move_test2.launch を move_test_rviz.launch にリネーム、およびseto_scararobot_descriptionからの呼び出しに変更
  * display_move_arm.launch を move_arm_rviz.launch にリネーム、およびseto_scararobot_descriptionからの呼び出しに変更
  * display_move_test2.launch を move_arm_hardware.launch にリネーム、およびseto_scararobot_descriptionからの呼び出しに変更
* ...
