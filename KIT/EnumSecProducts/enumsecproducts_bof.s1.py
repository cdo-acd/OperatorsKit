from typing import List, Tuple
from outflank_stage1.task.base_bof_task import BaseBOFTask
from outflank_stage1.task.enums import BOFArgumentEncoding

class FindFileBOF(BaseBOFTask):
    def __init__(self):
        super().__init__("enumsecproducts")

        self.parser.add_argument(
            "--hostname",
            default=str(),
            help=f"Host to enumerate security products on."
        )

    def _encode_arguments_bof(
        self, arguments: List[str]
    ) -> List[Tuple[BOFArgumentEncoding, str]]:
        parser_arguments = self.parser.parse_args(arguments)

        return [
            (BOFArgumentEncoding.STR, parser_arguments.hostname)
        ]
    