#include "minunit.h"
#include <sdb.h>
#include <fcntl.h>

static int foreach_delete_cb(void *user, const char *key, const char *val) {
	if (strcmp (key, "bar")) {
		sdb_unset (user, key, 0);
	}
	return 1;
}

bool test_sdb_foreach_delete(void) {
	Sdb *db = sdb_new (NULL, NULL, false);
	sdb_set (db, "foo", "bar", 0);
	sdb_set (db, "bar", "cow", 0);
	sdb_set (db, "low", "bar", 0);
	sdb_foreach (db, foreach_delete_cb, db);

	mu_assert ("Item not deleted", !(int)(size_t)sdb_const_get (db, "foo", NULL));
	mu_assert ("Item not deleted", (int)(size_t)sdb_const_get (db, "bar", NULL));

	sdb_free (db);
	mu_end;
}

bool test_sdb_list_delete(void) {
	Sdb *db = sdb_new (NULL, NULL, false);
	sdb_set (db, "foo", "bar", 0);
	sdb_set (db, "bar", "cow", 0);
	sdb_set (db, "low", "bar", 0);
	SdbList *list = sdb_foreach_list (db);
	SdbListIter *iter;
	SdbKv *kv;
	ls_foreach (list, iter, kv) {
		//printf ("--> %s\n", kv->key);
		sdb_unset (db, kv->key, 0);
	}
	ls_free (list);
	mu_assert ("List is empty", !ls_length (sdb_foreach_list (db)));
	sdb_free (db);
	mu_end;
}

bool test_sdb_delete_none(void) {
	Sdb *db = sdb_new (NULL, NULL, false);
	sdb_set (db, "foo", "bar", 0);
	sdb_set (db, "bar", "cow", 0);
	sdb_set (db, "low", "bar", 0);
	sdb_unset (db, "fundas", 0);
	sdb_unset (db, "barnas", 0);
	sdb_unset (db, "bar", 0);
	sdb_unset (db, "pinuts", 0);
	SdbList *list = sdb_foreach_list (db);
	mu_assert_eq ((int)ls_length (sdb_foreach_list (db)), 2, "Unmatched rows");
	sdb_free (db);
	mu_end;
}

bool test_sdb_delete_alot(void) {
	Sdb *db = sdb_new (NULL, NULL, false);
	const int count = 2048;
	char key[128];
	int i;

	for (i = 0; i < count; i++) {
		sdb_set (db, sdb_fmt (0, "key.%d", i), "bar", 0);
	}
	for (i = 0; i < count; i++) {
		sdb_unset (db, sdb_fmt (0, "key.%d", i), 0);
	}
	SdbList *list = sdb_foreach_list (db);
	mu_assert_eq ((int)ls_length (sdb_foreach_list (db)), 0, "Unmatched rows");
	sdb_free (db);

	mu_end;
}

bool test_sdb_milset(void) {
	int i = 0;
	const int MAX = 19999999;
	Sdb *s = sdb_new0 ();
	sdb_set (s, "foo", "bar", 0);
	for (i = 0; i < MAX ; i++) {
		if (!sdb_set (s, "foo", "bar", 0)) {
			mu_assert ("milset: sdb_set failed", 0);
			break;
		}
	}
	sdb_free (s);
	mu_end;
}

bool test_sdb_milset_random(void) {
	int i = 0;
	const int MAX = 19999999;
	bool solved = true;
	Sdb *s = sdb_new0 ();
	sdb_set (s, "foo", "bar", 0);
	for (i = 0; i < MAX ; i++) {
		char *v = sdb_fmt (0, "bar%d", i);
		if (!sdb_set (s, "foo", v, 0)) {
			solved = false;
			break;
		}
	}
	mu_assert ("milset: sdb_set", solved);
	sdb_free (s);
	mu_end;
}

bool test_sdb_namespace(void) {
	bool solved = false;
	const char *dbname = ".bar";
	Sdb *s = sdb_new0 ();
	if (!s) {
		return false;
	}
	unlink (dbname);
	Sdb *n = sdb_ns (s, dbname, 1);
	if (n) {
		sdb_set (n, "user.pancake", "pancake foo", 0);
		sdb_set (n, "user.password", "jklsdf8r3o", 0);
		/* FIXED BUG1 ns_sync doesnt creates the database file */
		sdb_ns_sync (s);
		// sdb_sync (n);
		/* FIXED BUG2 crash in free */
	 	sdb_free (s);

		int fd = open (dbname, O_RDONLY);
		if (fd != -1) {
			close (fd);
			solved = true;
		}
		unlink (dbname);
	}
	mu_assert ("namespace sync", solved);
	return solved;
}

int all_tests() {
	// XXX two bugs found with crash
	mu_run_test (test_sdb_namespace);
	mu_run_test (test_sdb_foreach_delete);
	mu_run_test (test_sdb_list_delete);
	mu_run_test (test_sdb_delete_none);
	mu_run_test (test_sdb_delete_alot);
	mu_run_test (test_sdb_milset);
	mu_run_test (test_sdb_milset_random);
	return tests_passed != tests_run;
}

int main(int argc, char **argv) {
	return all_tests ();
}
