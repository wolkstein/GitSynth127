import pyesedb


esedb_file = pyesedb.file()

esedb_file.open("preset.edb")

esedb_file.close()