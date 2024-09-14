db: db.c
		gcc *.c -o db

run: db
		./db

test: db
		bundle exec rspec