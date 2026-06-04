# Library System – OOP + Docker

> C++ OOP библиотечна система, контейнеризирана с Docker и Docker Compose.

---

## Структура на проекта

```
library-docker/
├── backend/
│   ├── Author.h          # клас Author
│   ├── Book.h            # клас Book (static брояч)
│   ├── Member.h          # клас Member
│   ├── Loan.h            # клас Loan
│   ├── Library.h         # клас Library (агрегира останалите)
│   ├── main.cpp          # точка на влизане + PostgreSQL демо
│   ├── Dockerfile        # multi-stage build (builder → runtime)
│   └── .dockerignore
├── db/
│   └── init/
│       └── 01_schema.sql # SQL схема (автоматично изпълнява се при първи старт)
├── compose.yml           # Docker Compose конфигурация
├── .gitignore
└── README.md
```

---

## Компоненти / услуги

### 1. `db` – PostgreSQL 16

| Параметър      | Стойност           |
|----------------|--------------------|
| Image          | `postgres:16-alpine` |
| Container      | `library_db`       |
| База данни     | `library`          |
| Потребител     | `librarian`        |
| Парола         | `secret`           |
| Порт (вътрешен)| `5432`             |

Съхранява данни за книги, автори, членове и заеми в четири таблици:
`authors`, `books`, `members`, `loans`.

Инициализационният скрипт `db/init/01_schema.sql` се изпълнява **автоматично** при първото стартиране на контейнера.

---

### 2. `backend` – C++ приложение

| Параметър  | Стойност                                  |
|------------|-------------------------------------------|
| Base image | `debian:bookworm-slim` (build + runtime)  |
| Container  | `library_backend`                         |
| Изграден с | `g++ -std=c++17`, свързва `libpq`         |

Приложението:
1. Изпълнява пълното **in-memory OOP демо** (оригиналната логика с `Library`, `Book`, `Loan` и др.).
2. Свързва се с PostgreSQL чрез `DATABASE_URL` и:
   - Създава таблиците (`IF NOT EXISTS`).
   - Записва примерни данни (автори, книги, членове, заем).
   - Извежда съдържанието на таблиците.

---

## Комуникация между услугите

```
┌─────────────────────┐          libpq / TCP:5432         ┌──────────────────┐
│  backend            │ ───────────────────────────────▶  │  db              │
│  (library_backend)  │     DATABASE_URL=host=db ...      │  (library_db)    │
└─────────────────────┘                                    └──────────────────┘
```

- Docker Compose създава вътрешна мрежа; `backend` достига `db` по hostname `db`.
- `depends_on: db: condition: service_healthy` гарантира, че базата е готова преди `backend` да стартира.
- `DATABASE_URL` се подава като environment variable.

---

## Как се изграждат и стартират контейнерите

### Изисквания

- [Docker Desktop](https://www.docker.com/products/docker-desktop/) ≥ 24 **или** Docker Engine + Compose plugin

### Бърз старт

```bash
# 1. Клонирай хранилището
git clone https://github.com/YOUR_USERNAME/library-docker.git
cd library-docker

# 2. Изгради образите и стартирай
docker compose up --build

# 3. Спри контейнерите
docker compose down

# 4. Спри и изтрий volume-а с данни (чист старт)
docker compose down -v
```

### Само изграждане на образа (без стартиране)

```bash
docker compose build
```

### Преглед на логовете

```bash
docker compose logs backend   # изход от C++ приложението
docker compose logs db        # PostgreSQL логове
```

### Свързване към базата с psql

```bash
docker exec -it library_db psql -U librarian -d library
```

---

## Docker Hub

| Компонент | Образ                                                    |
|-----------|----------------------------------------------------------|
| backend   | `https://hub.docker.com/r/YOUR_USERNAME/library-backend` |

> `db` използва официалния `postgres:16-alpine` образ – не е нужно публикуване.

### Публикуване на образа

```bash
# Замени YOUR_USERNAME с твоето Docker Hub потребителско име
docker login
docker compose build
docker push YOUR_USERNAME/library-backend:latest
```

---

## Промяна на паролата (production)

Редактирай `compose.yml` и смени:
```yaml
POSTGRES_PASSWORD: secret
```
и съответно в `DATABASE_URL` на `backend`.

---


