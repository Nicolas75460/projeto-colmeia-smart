import z from "zod";

export const baseDeviceSchema = z.object({
  name: z
    .string({
      required_error: "O nome do dispositivo é obrigatório.",
    })
    .min(2, {
      message: "O nome deve ter pelo menos 2 caracteres.",
    })
    .max(100, {
      message: "O nome deve ter no máximo 100 caracteres.",
    })
    .trim()
    .refine((val) => val.length > 0, {
      message: "O nome não pode estar vazio.",
    }),

  preset: z
    .string({
      required_error: "A predefinição é obrigatória.",
    })
    .min(1, {
      message: "Selecione uma predefinição.",
    }),
});

export type baseDeviceType = z.infer<typeof baseDeviceSchema>;