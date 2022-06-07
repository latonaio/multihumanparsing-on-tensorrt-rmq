# multihumanparsing-on-tensorrt-rmq
multihumanparsing-on-tensorrt-rmq は、TensorRT 上で Multi-Human Parsing の AIモデル を動作させるマイクロサービスです。  
multihumanparsing-on-tensorrt-rmq は、AI からのアウトプットとして、1/10/100ミリ秒のタイムサイクルで、マイクロサービス間の効率的・安定的処理が可能な RabbitMQ へメッセージを JSON 形式で出力します。    


## 動作環境
- Multi-Human Parsing
- C
- C++
- Docker
- TensorRT Runtime

## Multi-Human Parsingについて
[Multi-Human Parsing](https://github.com/ZhaoJ9014/Multi-Human-Parsing) は、画像内の身体の部分や衣服のアイテムに属する意味的に一貫した領域に分割し、各ピクセルに意味的な部分のラベルと当該領域が属するIDを割り当てるAIモデルです。  
[Multi-Human Parsing](https://github.com/ZhaoJ9014/Multi-Human-Parsing) は、特徴抽出にResNet18を使用しています。

## 動作手順
### Dockerコンテナの起動
Makefile に記載された以下のコマンドにより、Multi-Human Parsing の Dockerコンテナ を起動します。
```
docker-run: ## Docker ストリーミング用コンテナを建てる
	./docker/run.sh
```
### ストリーミングの開始
以下のコマンドにより、DeepStream 上の Multi-Human Parsing でストリーミングを開始します。  

```
segnet --network=fcn-resnet18-mhp --alpha=90 /dev/video0
```

## engineファイルについて
本レポジトリのengineファイルは、multihumanparsing-on-tensorrt/data/networks/FCN-ResNet18-MHP-512x320 にあります。  
なお、Multi-Human Parsingの NVIDIA Jetson用の engineファイルは、[jetson-inference](https://github.com/dusty-nv/jetson-inference) からダウンロードできます。  

## RabbitMQ への JSON Output
multihumanparsing-on-tensorrt-rmq は、Outputとして、RabbitMQ へのメッセージを以下のようなJSON形式で出力します。  
サンプルのJSONファイル は、Outputs フォルダ内の sample.json にあります。  

以下のアウトプットにおいて、fnum は streaming 開始からの フレーム数、classify は 1フレームの画像を、縦に10、横に16に分割し、分割されたそれぞれの領域の箇所と、推論で判断した物体に紐付けられた番号を示したものです。  
具体的に yは、分割された画像の上から何行目かを表し、classesは、配列の箇所が、分割された画像領域の何列目かを表しており、記載されているクラス番号が、推論で判断した物体に紐付けられた番号を表しています。    
例えば、"y": 1 における "classes": 5は、分割された画像の、上から1行目（"y":1）の、左から12列目の領域が、物体5（"classes":5, arm）と推論されたということになります。  

クラス番号と物体の対応については、data/network/FCN-ResNet18-MHP-512x320/classes.txt を参照してください。  

```
{
    "fnum": 1568, 
    "classify": [
        {"y": 0, "classes": [0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0]}, 
        {"y": 1, "classes": [0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 5, 0, 0, 0, 0]}, 
        {"y": 2, "classes": [0, 0, 0, 0, 3, 2, 2, 3, 0, 0, 0, 5, 0, 0, 0, 0]}, 
        {"y": 3, "classes": [0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 5, 4, 0, 0, 0]}, 
        {"y": 4, "classes": [0, 0, 0, 0, 7, 2, 2, 0, 0, 0, 0, 4, 4, 0, 0, 0]}, 
        {"y": 5, "classes": [0, 0, 0, 0, 6, 10, 10, 6, 6, 0, 0, 0, 4, 0, 0, 0]}, 
        {"y": 6, "classes": [0, 0, 0, 6, 6, 6, 10, 6, 6, 6, 6, 0, 0, 0, 0, 0]}, 
        {"y": 7, "classes": [0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0]}, 
        {"y": 8, "classes": [0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0]}, 
        {"y": 9, "classes": [0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 0, 11, 11, 0, 0, 0]}
    ]
}
```

