PyTorch DGCNN
=============

About
-----

Official Code Repository for “Enhancing Attacks on Logic Locking with LLMs 

and Evolutionary Key Search”.

## Usage

### Phase-I

##### a. Attack XOR-based RLL

To train GNN:

```
python Main.py --split-val --use-dis --save-model --file-name c1908 --links-name link.txt --dataset-name dataset-I  --batch_size 64  --filename Release_c1908_result_b64_h2_fan_6layers_hd64.txt  --hidden_dim 64 --num_layers 6 > Release_log_c1908_b64_h2_6layers_hd64.txt
```

Get the prediction of GNN:

```
python Main.py --split-val --use-dis --only-predict --file-name c1908 --links-name link.txt --dataset-name dataset-I --batch_size 64 --hidden_dim 64 --num_layers 6 > Release_predict_log_c1908_b64_h2_6layers_hd64.txt
```

Process the key prediction result:

```
g++ result.cpp -o result
```

```
./result c1908
```

The result will be save in reslut.txt.

##### b. Attack D-MUX

Install the default GNN model:

    cd Phase-I/Mux
    git clone https://github.com/muhanzhang/pytorch_DGCNN
    cd pytorch_DGCNN/lib
    make -j4

To train GNN and get prediction without synthetic data:

    python Main_origin.py --file-name c1908_K128_DMUX --train-name links_train.txt  --test-name links_test.txt --testneg-name link_test_n.txt --hop 3  --save-model > Log_train_c1908_DMUX_K128.txt
    
    python Main_origin.py  --file-name c1908_K128_DMUX --train-name links_train.txt  --test-name links_test.txt --hop 3  --only-predict > Log_pos_predict_c1908_DMUX_K128.txt
    
    python Main_origin.py  --file-name c1908_K128_DMUX --train-name links_train.txt  --test-name  link_test_n.txt --hop 3  --only-predict > Log_neg_predict_c1908_DMUX_K128.txt

To train GNN and get prediction with synthetic data:

    
    python Main_syndata.py --file-name c1908_K128_DMUX --circuit-name c1908 --train-name links_train.txt  --test-name links_test.txt --testneg-name link_test_n.txt --hop 3  --save-model > Log_train_c1908_DMUX_K128.txt
    
    python Main_syndata.py  --file-name c1908_K128_DMUX --circuit-name c1908 --train-name links_train.txt  --test-name links_test.txt --hop 3  --only-predict > Log_pos_predict_c1908_DMUX_K128.txt
    
    python Main_syndata.py  --file-name c1908_K128_DMUX --circuit-name c1908 --train-name links_train.txt  --test-name  link_test_n.txt --hop 3  --only-predict > Log_neg_predict_c1908_DMUX_K128.txt

Parse the predictions

    perl break_DMUX.pl c1908_K128_DMUX 0.01 3

### Phase-II

To compile the project:

```
cd Phase-II
make
```

To run BDE and get result:

```
./BDE RLL c1355
```

RLL is the locking scheme of circuit, you can also choose IOLTS14 or ToC13.

The result will be save in timeRecord.txt and result.txt.

## Acknowledgement

We owe many thanks to Lilas Alrahis for making OMLA and MuxLink available.
