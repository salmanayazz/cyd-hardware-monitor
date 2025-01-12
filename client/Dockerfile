FROM python:3.13

RUN apt-get update

RUN apt-get install -y bluez bluetooth

COPY requirements-secondary.txt .

RUN pip install --no-cache-dir -r requirements-secondary.txt

COPY . .

CMD [ "python", "-u", "main.py" ]