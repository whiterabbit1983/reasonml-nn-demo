let outBinaryPic = (data: array(int), width: int, height: int): unit => {
  for (lineNum in 0 to height - 1) {
    let line =
      data->Array.sub(lineNum * width, width)
      |> Array.fold_left(
           (acc, val_) =>
             acc
             ++ (
               if (val_ == 0) {
                 " ";
               } else {
                 "O";
               }
             ),
           "",
         );
    Js.log(line);
  };
};
