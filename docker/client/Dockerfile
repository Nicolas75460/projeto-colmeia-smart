# ---- Stage 1: Build ----
FROM node:18-alpine AS builder

WORKDIR /app

# Copia package.json e package-lock.json primeiro (cache melhor)
COPY client/package*.json ./

# Instala TODAS dependências (incluindo dev)
RUN npm install

# Copia o resto do código
COPY client ./

# Faz o build do Next
RUN npm run build

# ---- Stage 2: Run ----
FROM node:18-alpine AS runner

WORKDIR /app

ENV NODE_ENV=production

# Copia apenas pacotes e arquivos de build do stage anterior
COPY --from=builder /app/package*.json ./
COPY --from=builder /app/node_modules ./node_modules
COPY --from=builder /app/.next ./.next
COPY --from=builder /app/public ./public

EXPOSE 3000

CMD ["npm", "start"]
