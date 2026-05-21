import z from "zod"
import { baseDeviceSchema } from "../shared/base-validator";

export const editDeviceSchema = baseDeviceSchema.extend({
  id: z.string({ required_error: "Campo obrigatório" }).uuid("Formato de ID errado"),
  is_active: z.boolean({ required_error: "Campo obrigatório" }),
});

export type EditDeviceType = z.infer<typeof editDeviceSchema>;
