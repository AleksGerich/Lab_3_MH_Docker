FROM gcc:10.4
	
WORKDIR /app

COPY . .

ENV TEMP_VALUE="Test"

RUN g++ -o myapp lab_3_MH_Docker.cpp

CMD ["./myapp"]