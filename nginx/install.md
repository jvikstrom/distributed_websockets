sudo apt-get install nginx

#Locations for nginx.conf: /usr/local/nginx/conf, /etc/nginx, or /usr/local/etc (mine is /etc/nginx)
sudo mv /etc/nginx/nginx.conf.bkp
sudo mv nginx.conf /etc/nginx/nginx.conf
