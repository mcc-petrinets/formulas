#! /bin/bash

for sn in ~/Téléchargements/BenchKit/INPUTS/*-COL-*
do
  pt=${sn/-COL-/-PT-}
  if [ -d "${pt}" ]
  then
    echo "Working on $(basename ${sn}) -- $(basename ${pt})"
    echo "Fixing model..."
    php mcc.php fix-model ${sn}/model.pnml
    php mcc.php fix-model ${pt}/model.pnml
    echo "Checking consistency..."
    php mcc.php check-unfolding ${sn}/model.pnml ${pt}/model.pnml
  fi
done

