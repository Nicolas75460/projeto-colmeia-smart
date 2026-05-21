import z from "zod";
import { baseDeviceSchema } from "../shared/base-validator";

export const newDeviceSchema = baseDeviceSchema.extend({});

export type newDeviceType = z.infer<typeof newDeviceSchema>;
