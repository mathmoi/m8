FROM python:latest

# Setup the system
WORKDIR /app

# Setupe lichess-bot
RUN git clone https://github.com/lichess-bot-devs/lichess-bot.git .
RUN python -m pip install -r requirements.txt

COPY ./lichess-bot/config.yml .

# Setup the engine
WORKDIR /app/engines
COPY build/apps/m8/m8 .
COPY lichess-bot/m8.json .

# Configure the entry point
WORKDIR /app
CMD ["python", "lichess-bot.py"]