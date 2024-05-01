## Setup instructions for running on Google Cloud Compute Machines.

sudo apt-get update

# install git
sudo apt install git-all
# install pip3
sudo apt install python3-pip

# install bazel
sudo apt install apt-transport-https curl gnupg -y
curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor >bazel-archive-keyring.gpg
sudo mv bazel-archive-keyring.gpg /usr/share/keyrings
echo "deb [arch=amd64 signed-by=/usr/share/keyrings/bazel-archive-keyring.gpg] https://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list
sudo apt update && sudo apt install bazel
bazel --version


git clone --recurse-submodules https://github.com/ParAlg/ParClusterers.git
git checkout vldb2025 
cd ParClusterers

# compile
bazel build //clusterers:cluster-in-memory_main
bazel build //clusterers:stats-in-memory_main

# install python packages
pip3 install -r requirements.txt --break-system-packages

# install latex for plotting
sudo apt-get install texlive-latex-base texlive-fonts-recommended texlive-fonts-extra texlive-latex-extra
sudo apt-get install dvipng

# Now, create a folder ParClusterers/pcbs_vldb_2025 and put data into it.
# The command here assumes that you have data in `pcbs_vldb_2025` google storage bucket.
# follow instructions here to install gcsfuse: https://cloud.google.com/storage/docs/gcsfuse-install
mkdir pcbs_vldb_2025
gcsfuse --implicit-dirs pcbs_vldb_2025 "$HOME/ParClusterers/pcbs_vldb_2025"
