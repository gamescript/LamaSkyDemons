#LucKey Productions GameDev install script
#sudo apt-get install -y git qtcreator blender inkscape gimp qjackctl hydrogen rakarrack audacity
#sudo apt-get update
#sudo apt-get upgrade -y

if [ ! -d ~/GameDev ]
then
  mkdir ~/GameDev
fi

cd ~/GameDev

if [ ! -d Urho3D ]
then
  git clone https://github.com/Urho3D/Urho3D
  cd Urho3D
  cmake .
  make
  ./cmake_generic.sh
  cd ..
fi

if [ ! -d Urho3DQtemplate ]
then
  git clone https://github.com/LucKeyProductions/Urho3DQtemplate
  cd Urho3DQtemplate
  ln -s ../Urho3D/
  cd ..

  mkdir build-Urho3DQtemplate-Desktop-Debug
  cd build-Urho3DQtemplate-Desktop-Debug
  ln -s ../Urho3D/bin/Data
  ln -s ../Urho3D/bin/CoreData
  ln -s ../Urho3DQtemplate/Resources
  cd ..

  mkdir build-Urho3DQtemplate-Desktop-Release
  cd build-Urho3DQtemplate-Desktop-Release
  ln -s ../Urho3D/bin/Data
  ln -s ../Urho3D/bin/CoreData
  ln -s ../Urho3DQtemplate/Resources
  cd ..
fi

cd Urho3DQtemplate
git pull origin master
qtcreator Urho3DQtemplate.pro
