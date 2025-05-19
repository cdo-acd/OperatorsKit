from typing import List, Tuple

from outflank_stage1.task.base_bof_task import BaseBOFTask
from outflank_stage1.task.enums import BOFArgumentEncoding



class CredPromptBof(BaseBOFTask):
    def __init__(self):
        super().__init__("credprompt")

        self.parser.description = (
            "Prompt the user for their credentials. Specify the title and message of the popup box and also how long it'll run for."
        )

        self.parser.add_argument("title", help="Title of the prompt popup.")
        self.parser.add_argument("message", help="Message of the prompt popup.")
        self.parser.add_argument("timer", type=int, help="Timeout in seconds.")

    def _encode_arguments_bof(
        self, arguments: List[str]
    ) -> List[Tuple[BOFArgumentEncoding, str]]:
        parser_arguments = self.parser.parse_args(arguments)
        title = parser_arguments.title.strip("'").strip('"')
        message = parser_arguments.message.strip("'").strip('"')
        timer = int(parser_arguments.timer)

        return [
            (BOFArgumentEncoding.WSTR, title),
            (BOFArgumentEncoding.WSTR, message),
            (BOFArgumentEncoding.INT, timer),
        ]