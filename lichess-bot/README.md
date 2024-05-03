# How to create the docker image to play on lichess

## Add the key in the config.yml
On the first line of the [config.yml](config.yml) file, change the placeholder value for the Lichess OAuth2 Token of the engine account.

## Build the image
cd to the root of the repo and run the following command.
```bash
docker build . -f lichess-bot/Dockerfile  -t m8:latest
```

## Run the image
```bash
docker run -d --restart=always --name m8 --rm m8
```