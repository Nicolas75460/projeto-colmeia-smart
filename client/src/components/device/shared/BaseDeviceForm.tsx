"use client";
import React from "react";
import {
  FormControl,
  FormDescription,
  FormField,
  FormItem,
  FormLabel,
  FormMessage,
} from "../../ui/form";
import { Input } from "../../ui/input";
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from "../../ui/select";
import { useFormContext } from "react-hook-form";

interface BaseDeviceFormProps {
  disabled?: boolean;
}

const PRESET_OPTIONS = [
  { value: "apis-mellifera", label: "Apis mellifera" },
  { value: "apis-cerana", label: "Apis cerana" },
  { value: "apis-dorsata", label: "Apis dorsata" },
  { value: "apis-florea", label: "Apis florea" },
  { value: "custom", label: "Personalizado" },
];

export const BaseDeviceForm = ({ disabled = false }: BaseDeviceFormProps) => {
  const form = useFormContext();

  return (
    <div className="space-y-6 ">
      <FormField
        control={form.control}
        name="name"
        render={({ field }) => (
          <FormItem>
            <FormLabel>Nome</FormLabel>
            <FormControl>
              <Input
                placeholder="Colmeia BEEthoven"
                disabled={disabled}
                {...field}
              />
            </FormControl>
            <FormMessage />
          </FormItem>
        )}
      />

      <FormField
        control={form.control}
        name="preset"
        render={({ field }) => (
          <FormItem>
            <FormLabel>Predefinição</FormLabel>
            <Select
              disabled={disabled}
              onValueChange={field.onChange}
              value={field.value}
            >
              <FormControl>
                <SelectTrigger>
                  <SelectValue placeholder="Selecione uma predefinição" />
                </SelectTrigger>
              </FormControl>
              <SelectContent>
                {PRESET_OPTIONS.map((preset) => (
                  <SelectItem key={preset.value} value={preset.value}>
                    {preset.label}
                  </SelectItem>
                ))}
              </SelectContent>
            </Select>
            <FormMessage />
          </FormItem>
        )}
      />
    </div>
  );
};