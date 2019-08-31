module Synaptic = {
  module Layer = {
    type t;

    [@bs.module "synaptic"] [@bs.new] external make: int => t = "Layer";
    [@bs.send] external project: (t, t) => unit = "project";
  };

  module Network = {
    type t;

    [@bs.module "synaptic"] [@bs.new] external make: Js.t('a) => t = "Network";
    [@bs.send] external activate: (t, array('a)) => array('b) = "activate";
  };

  module Trainer = {
    type t;

    [@bs.module "synaptic"] [@bs.new] external make: Network.t => t = "Trainer";
    [@bs.send] external train: (t, array(Js.t('a)), Js.t('b)) => unit = "train";
    [@bs.module "synaptic"] [@bs.val] [@bs.scope ("Trainer", "cost")]
    external crossEntropy: (array(float), array(float)) => float = "CROSS_ENTROPY";
  };
};

let main = (): unit => {
  // prepare sets
  let mnist = Mnist.make(700, 20);
  let trainingSet = mnist->Mnist.get_training;
  let testSet = mnist->Mnist.get_test;

  // create layers
  let inputLayer = Synaptic.Layer.make(784);
  let hiddenLayer = Synaptic.Layer.make(100);
  let outputLayer = Synaptic.Layer.make(10);
  open Synaptic.Layer;

  // combine layers
  inputLayer->project(hiddenLayer);
  hiddenLayer->project(outputLayer);

  // create and train network
  let network = Synaptic.Network.make({"input": inputLayer, "hidden": [|hiddenLayer|], "output": outputLayer});
  let trainer = Synaptic.Trainer.make(network);
  Synaptic.Trainer.(
    trainer->train(
      trainingSet,
      {"rate": 0.2, "iterations": 20, "error": 0.1, "shuffle": true, "log": 1, "cost": crossEntropy},
    )
  );

  // test network
  let testOutput = Synaptic.Network.(network->activate(testSet[0]->Mnist.get_input));
  Js.log2("predicted output", testOutput);
  Js.log2("labeled output", testSet[0]->Mnist.get_output);
};

main();
