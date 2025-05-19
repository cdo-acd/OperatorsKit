from typing import List, Tuple
from outflank_stage1.task.base_bof_task import BaseBOFTask
from outflank_stage1.task.enums import BOFArgumentEncoding

class FindFileBOF(BaseBOFTask):
    def __init__(self):
        super().__init__("findfile")

        self.parser.description = "Search for matching files based on a word, extention or keyword."
        self.parser.epilog = """Synopsis: findfile <PATH> <SEARCH PATTERN> [KEYWORD]
        Examples: 
            findfile C:\\Users\\RTO\\Documents *.xlsx
            findfile C:\\Users\\RTO *login*.* password"""

        self.parser.add_argument(
            "path",
            help=f"Path to the directory to start searching recursively."
        )

        self.parser.add_argument(
            "search",
            help=f"Filename pattern to search for (supports wildcard as *)."
        )

        self.parser.add_argument(
            "--keyword",
            default=str(),
            help=f"Keywords to search inside the file for (supports wildcard as *)."
        )

    def _encode_arguments_bof(
        self, arguments: List[str]
    ) -> List[Tuple[BOFArgumentEncoding, str]]:
        parser_arguments = self.parser.parse_args(arguments)

        return [
            (BOFArgumentEncoding.STR, parser_arguments.path),
            (BOFArgumentEncoding.STR, parser_arguments.search),
            (BOFArgumentEncoding.STR, parser_arguments.keyword),
        ]
    