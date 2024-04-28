FROM gcc:latest

WORKDIR /app

COPY . .

RUN g++ -std=c++17 -o ws *.cpp

CMD [ "./ws" ]
