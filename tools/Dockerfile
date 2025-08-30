# Dockerfile com múltiplos estágios
# Estágio 1: Dependências base
FROM ubuntu:22.04 AS base-deps
RUN apt-get update && \
	apt-get install \
	cmake \
	wget \
	g++ \
	gdb \
	netcat \
	git \
	curl \
	uuid-dev \
	iproute2 \
	-y && \
    apt-get clean

# Estágio 2: Dependências de desenvolvimento
FROM base-deps AS dev-deps
RUN apt-get install \
	libmysqlclient-dev \
	libcurl4-openssl-dev \
	librdkafka-dev \
	pkg-config \
	libglib2.0-dev \
	libpq-dev \
	libhiredis-dev \
	jq \
	-y && \
    apt-get clean

# Estágio 3: Compilação de bibliotecas externas
FROM dev-deps AS external-libs
WORKDIR /development

# CivetWeb
RUN git clone https://github.com/civetweb/civetweb.git && \
	mkdir -p civetweb/_build && \
	cd civetweb/_build && \
	cmake -DCMAKE_C_FLAGS="-fPIC" -DCMAKE_CXX_FLAGS="-fPIC" .. && \
	make install && \
	rm -rf /development/civetweb

# SQLite
RUN wget https://www.sqlite.org/2024/sqlite-autoconf-3450100.tar.gz && \
	tar -xvf sqlite-autoconf-3450100.tar.gz && \
	cd sqlite-autoconf-3450100 && \
    ./configure --prefix=/usr/local && \
	make install && \
	cd .. && \
    rm -rf sqlite-autoconf-3450100.tar.gz sqlite-autoconf-3450100

# MQTT
RUN git clone https://github.com/eclipse/paho.mqtt.c.git && \
	cd paho.mqtt.c && \
	make install && \
	cd .. && \
	rm -rf paho.mqtt.c

# Estágio 4: Compilação do SAT
FROM external-libs AS sat-build
RUN git clone https://github.com/NakedSolidSnake/sat && \
	mkdir -p sat/build && \
	cd sat/build && \
	cmake \
	-DSAT_WEBSERVER=ON \
	-DSAT_CURL=ON \
	-DSAT_PROPERTIES=ON \
	-DSAT_JSON=ON \
	-DSAT_KAFKA=ON \
	-DSAT_MQTT=ON \
	-DSAT_MYSQL=ON \
	-DSAT_POSTGRES=ON \
	-DSAT_REDIS=ON \
	-DSAT_SQLITE=ON \
	.. && \
	make install && \
	rm -rf /development/sat && \
	ldconfig

# Estágio 5: Imagem final
FROM sat-build AS final
RUN git clone https://github.com/NakedSolidSnake/sat_scaffolding && \
	mkdir -p sat_scaffolding/build && \
	cd sat_scaffolding/build && \
	cmake .. && \
	make install && \
	rm -rf /development/sat_scaffolding

RUN useradd -ms /bin/bash developer && chmod 0777 /development

USER developer

EXPOSE 1234

CMD [ "/bin/tail", "-f", "/dev/null" ]
