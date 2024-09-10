sudo su
cd /home/user/Desktop
apt install libomp-devel
apt install git
git clone https://github.com/Shashank-Shet/OpenMP-man-pages.git
cd OpenMP-man-pages
bash create_zip.sh
mkdir /home/man
mkdir /home/man/man3
mv /home/user/Desktop/OpenMP-man-pages/Zipped\ Pages/* /home/man/man3/
cd /home
echo 'export MANPATH=$HOME/man:$MANPATH' >> ~/.bashrc
source ~/.bashrc
man omp_get_wtime
