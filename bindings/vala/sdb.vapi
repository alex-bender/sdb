namespace SimpleDB {
	[Compact]
	[CCode (cheader_filename="sdb.h", cname="struct sdb_t", cprefix="sdb_", free_function="sdb_free")]
	public class Sdb {
		/* lifecycle */
		public Sdb (string? path=null, string? file=null, bool locked=false);
		public bool sync ();
		/* query */
		public bool query(string cmd);
		public string querys(string? buf=null, int len=0, string cmd="");
		/* string */
		public bool exists (string key);
		public string @get (string key, out int? cas=null);
		public bool @add (string key, string val, uint32 cas=0);
		public bool @set (string key, string val, uint32 cas=0);
		/* arrays */
		public int array_len (string key);
		public string array_get (string key, int idx, uint32 *cas=null);
		public string array_set (string key, int idx, string val, uint32 *cas=null);
		public bool array_del (string key, int idx, uint32 cas=0);
		/* numeric */
		public bool num_exists (string key);
		public bool num_set (string key, uint64 num, uint32 cas=0);
		public uint64 num_get (string key, uint32 *cas = null);
		public uint64 num_inc (string key, uint64 n, uint32 cas=0);
		public uint64 num_dec (string key, uint64 n, uint32 cas=0);
		/* json */
		public string json_get (string key, string path, uint32 *cas = null);
		public string json_set (string key, string path, string val, uint32 cas);
		public int json_inc (string key, string path, int n, uint32 cas=0);
		public int json_dec (string key, string path, int n, uint32 cas=0);
		public static string json_indent (string json);
		public static string json_unindent (string json);
		/* remove */
		public bool del (string key, int cas=0);
		public void reset ();
		public void unlink ();
		/* time */
		public uint64 expire_get (string key);
		public bool expire_set (string key, uint64 time);
		public static uint64 now ();
	}
}
