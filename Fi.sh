#LucKey Productions "So you want to be a game dev?" install script for Xubuntu
sudo add-apt-repository -y ppa:ravefinity-project/ppa
sudo apt-get update
sudo apt-get remove -y ristretto abiword gnumetric
sudo apt-get install -y git qtcreator blender inkscape gimp qjackctl hydrogen rakarrack audacity libreoffice eog vlc redshift redshift-gtk vibrancy-colors
sudo apt-get update
sudo apt-get upgrade -y

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
qtcreator Urho3DQtemplate.pro
