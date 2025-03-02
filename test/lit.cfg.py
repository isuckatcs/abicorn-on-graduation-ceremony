import os
import lit.formats

config.name = "abicorn"
config.test_format = lit.formats.ShTest(False if os.name == 'nt' else True)

config.suffixes = ['.cc']
