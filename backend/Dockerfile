# ---- Build stage ----
FROM debian:bookworm-slim AS builder

RUN apt-get update && apt-get install -y --no-install-recommends \
        g++ \
        libpq-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .

RUN g++ -std=c++17 -O2 \
        -o library_app \
        main.cpp \
        $(pg_config --includedir | xargs -I{} echo -I{}) \
        -L$(pg_config --libdir) -lpq

# ---- Runtime stage ----
FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
        libpq5 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=builder /src/library_app .

CMD ["./library_app"]
