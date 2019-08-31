type t;

[@bs.module "mnist"] external make: (int, int) => t = "set";
[@bs.get] external get_training: t => array(Js.t('a)) = "training";
[@bs.get] external get_test: t => array(Js.t('a)) = "test";
[@bs.get] external get_input: Js.t('a) => array('b) = "input";
[@bs.get] external get_output: Js.t('a) => array('b) = "output";
[@bs.get_index] external get: (Js.t('a), int) => Js.t('b) = "";

let splitData = (data: array(Js.t('a))): (array(array(int)), array(array(int))) => {
  let inputs = Array.make(data |> Array.length, [||]);
  let outputs = Array.make(data |> Array.length, [||]);
  data
  |> Array.iteri((idx, x) => {
       inputs->Array.set(idx, x->get_input);
       outputs->Array.set(idx, x->get_output);
     });
  (inputs, outputs);
};
