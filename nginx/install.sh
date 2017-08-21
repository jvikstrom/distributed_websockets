sudo cp ./nginx.conf ./nginx.conf.build
sudo mv ./nginx.conf.build /etc/nginx/nginx.conf
sudo nginx -s reload