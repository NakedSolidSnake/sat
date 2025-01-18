FROM node

RUN npm install -g json-server

WORKDIR /app

EXPOSE 3000

ENTRYPOINT [ "json-server", "--watch", "db.json", "--port", "3000", "--host", "0.0.0.0" ]