module Tensorflow = {
  module Layer = {
    type t;

    module Dense = {
      [@bs.module "@tensorflow/tfjs-node"] [@bs.scope "layers"] external make: Js.t('a) => t = "dense";
    };

    module Flatten = {
      [@bs.module "@tensorflow/tfjs-node"] [@bs.scope "layers"] external make: Js.t('a) => t = "flatten";
    };
  };

  module Tensor = {
    type t;

    [@bs.module "@tensorflow/tfjs-node"] external make: array('a) => t = "tensor";
    [@bs.send] external print: t => unit = "print";
  };

  module Model = {
    type t;

    [@bs.module "@tensorflow/tfjs-node"] external sequential: unit => t = "sequential";
    [@bs.send] external add: (t, Layer.t) => unit = "add";
    [@bs.send] external compile: (t, Js.t('a)) => unit = "compile";
    [@bs.send] external fit: (t, Tensor.t, Tensor.t, Js.t('a)) => Js.Promise.t('b) = "fit";
    [@bs.send] external predict: (t, Tensor.t) => Tensor.t = "predict";
  };
};

let fitCallback = (_, logs: Js.t('a)): unit => {
  Js.log("Training finished: accuracy - " ++ logs##acc);
};

let main = () => {
  open Tensorflow;
  // define some constants
  let batchSize = 512;

  // prepare sets
  let mnist = Mnist.make(700, 20);
  let trainingSet = mnist->Mnist.get_training;
  let testSet = mnist->Mnist.get_test;
  let (trainXs_, trainYs_) = Mnist.splitData(trainingSet);
  let trainXs = Tensor.make(trainXs_);
  let trainYs = Tensor.make(trainYs_);
  let (testXs_, testYs_) = Mnist.splitData(testSet);
  let testXs = Tensor.make(testXs_);
  let testYs = Tensor.make(testYs_);

  // create the model
  let model = Model.sequential();
  open Model;
  model->add(Layer.Dense.make({"units": 100, "activation": "sigmoid", "inputShape": [|784|]}));
  model->add(Layer.Dense.make({"units": 10, "activation": "sigmoid"}));
  model->compile({"optimizer": "sgd", "loss": "categoricalCrossentropy", "metrics": [|"accuracy"|]});
  // train the model
  model->fit(
    trainXs,
    trainYs,
    {
      "batchSize": batchSize,
      "validationData": [|testXs, testYs|],
      "epochs": 20,
      "shuffle": true,
      "callbacks": {
        "fitCallback": fitCallback,
      },
    },
  )
  |> Js.Promise.then_(_ => {
       Js.log("Making test prediction...");
       let preds = model->predict(Tensor.make([|testXs_[0]|]));
       preds->Tensor.print;
       Tensor.make([|testYs_[0]|])->Tensor.print;
       Js.Promise.resolve();
     });
};

main();
