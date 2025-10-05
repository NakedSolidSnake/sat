FROM solidcris/development AS builder

FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y \
    libmysqlclient21 \
    libcurl4 \
    librdkafka1 \
    libglib2.0-0 \
    libpq5 \
    libhiredis0.14 \
    libsqlite3-0 \
    && apt-get clean && rm -rf /var/lib/apt/lists/*


COPY --from=builder /usr/local/lib/libsat.so /usr/local/lib/
COPY --from=builder /usr/local/lib/libcivetweb.a  /usr/local/lib/
COPY --from=builder /usr/local/lib/libpaho-mqtt3c.so.1.3 /usr/local/lib/

RUN ln -sf /usr/local/lib/libpaho-mqtt3c.so.1.3 /usr/local/lib/libpaho-mqtt3c.so.1 && \
    ln -sf /usr/local/lib/libpaho-mqtt3c.so.1 /usr/local/lib/libpaho-mqtt3c.so && ldconfig